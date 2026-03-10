#pragma once

#include <refl.hpp>
#include <locale>
#include <iostream>
#include <type_traits>
#include <unordered_set>
#include <variant>
#include <expected>

namespace Serialization
{
    class bad_json : public std::runtime_error
    {
    public:
        bad_json() : std::runtime_error("bad_json")
        {
        }
    };

    class json_schema_mismatch : public std::runtime_error
    {
    public:
        json_schema_mismatch() : std::runtime_error("json_schema_mismatch")
        {
        }
    };

    template <typename T>
    struct is_basic_string
    {
        static constexpr bool value = false;
    };

    template <class T, class Traits, class Alloc>
    struct is_basic_string<std::basic_string<T, Traits, Alloc>>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    static constexpr bool is_basic_string_v{is_basic_string<T>::value};

    // is std::array trait

    template <typename>
    struct is_std_array : std::false_type
    {
    };

    template <typename T, std::size_t N>
    struct is_std_array<std::array<T, N>> : std::true_type
    {
    };

    template <typename T>
    static constexpr bool is_std_array_v{is_std_array<T>::value};

    // is std::vector trait

    template <typename>
    struct is_std_vector : std::false_type
    {
    };

    template <typename T, typename Alloc>
    struct is_std_vector<std::vector<T, Alloc>> : std::true_type
    {
    };

    template <typename T>
    static constexpr bool is_std_vector_v{is_std_vector<T>::value};

    template <typename T>
    void toJsonInner(T&& in, std::string& out)
    {
        /*
        We have multiple possibilities at the beginning of parsing, T could be anything.
        While all types can't realistically be supported, the ones that can be broken down into formattable fundamentals should be.
        According to https://www.json.org/json-en.html values JSON supports are object, array, string, number, "true", "false", and "null".
        Even if those 7 types are unique, they are actually all strings with differing surrounding characters.
        Therefore, processing needs to go from any formattable type to a string representation and then reformatted as valid JSON.
        
        Logic flow for each case is as follows:
        object -> string can be done by recursively formatting through its members and assigning member names to keys and their stored value as the value.
        array -> string can be done by iteratively formatting each element and separating each entry by a comma.
        string -> string is formatted directly.
        number -> string is formatted directly.
        "true" -> string is formatted directly.
        "false" -> string is formatted directly.
        "null" -> string is formatted directly.
        
        For objects, this can be done either manually via template specialization, or through compile-time reflection as codegen, here the latter will be used by leveraging refl-cpp.
        For arrays, We consider bounded c-arrays and any objects containing an iterator as arrays.
        For booleans, we directly associate either "true" or "false"
        For null, nullptr_t translates to "null"
        For numbers, to_chars and from_chars is used.
        For strings, anything that accepts https://cppreference.net/cpp/io/basic_ostream/operator_ltlt.html is used.
        */

        using StrippedType = std::remove_cvref_t<T>;

        // JSON Boolean
        if constexpr (std::is_same_v<StrippedType, bool>)
        {
            out += in ? "true" : "false";
        }
        // JSON Null
        else if constexpr (std::is_same_v<StrippedType, nullptr_t>)
        {
            out += "null";
        }
        // JSON Number
        else if constexpr (std::is_integral_v<StrippedType>)
        {
            static constexpr std::size_t limit = std::numeric_limits<StrippedType>::digits10;
            size_t startIndex = out.size();
            char buffer[limit] = {};
            std::to_chars(buffer, buffer + limit, in);

            out.resize(out.size() + strlen(buffer));
            memcpy(out.data() + startIndex, buffer, strlen(buffer));
        }
        else if constexpr (std::is_floating_point_v<StrippedType>)
        {
            static constexpr std::size_t limit = std::numeric_limits<StrippedType>::max_digits10;
            size_t startIndex = out.size();
            char buffer[limit] = {};
            std::to_chars(buffer, buffer + limit, in);

            out.resize(out.size() + strlen(buffer));
            memcpy(out.data() + startIndex, buffer, strlen(buffer));
        }
        // JSON String
        else if constexpr (is_basic_string_v<StrippedType> || std::is_convertible_v<StrippedType, std::string>)
        {
            out += '"';
            out += in;
            out += '"';
        }
        // JSON Array
        else if constexpr (std::is_bounded_array_v<StrippedType>)
        {
            out += "[";
            for (size_t i = 0; i < std::extent_v<StrippedType>; ++i)
            {
                if (i != 0)
                {
                    out += ",";
                }
                toJsonInner(in[i], out);
            }
            out += "]";
        }
        else if constexpr (is_std_array_v<StrippedType> || is_std_vector_v<StrippedType>)
        {
            size_t i = 0;

            out += "[";
            for (auto val : in)
            {
                if (i != 0)
                {
                    out += ",";
                }
                toJsonInner(val, out);
                ++i;
            }
            out += "]";
        }
        // Reference
        else if constexpr (std::is_pointer_v<StrippedType>)
        {
            toJsonInner(*in, out);
        }
        // JSON Object
        else if constexpr (refl::trait::is_reflectable_v<StrippedType>)
        {
            out += "{";

            refl::util::for_each(refl::reflect(in).members, [&]<typename MD>(const MD member, size_t member_index)
            {
                if constexpr (refl::trait::is_field_v<MD>)
                {
                    using MT = MD::value_type;

                    if (member_index != 0)
                    {
                        out += ",";
                    }
                    out += '"';
                    out += refl::descriptor::get_display_name(member);
                    out += '"';
                    out += ":";
                    toJsonInner(refl::descriptor::invoke(member, in), out);
                }
            });
            out += "}";
        }
        // Unrepresentable
        else
        {
            static_assert(false, "Type cannot be serialized into JSON!");
        }
    }

    // We're using refl-cpp as the driver for serializing data into JSON, using it as a form of schema.

    template <typename T>
    std::string toJson(T&& in)
    {
        std::string out = "";
        toJsonInner(std::forward<T>(in), out);
        return out;
    }

    inline bool strip_prefix(std::string_view ref, std::string_view& target)
    {
        if (!target.starts_with(ref))
        {
            return false;
        }
        target.remove_prefix(ref.length());
        return true;
    }

    inline std::string_view extract_json_number(std::string_view& target)
    {
        size_t size = 0;

        // sign
        if (target.size() > size && target.at(size) == '-')
        {
            ++size;
        }

        // integer
        if (target.size() > size && target.at(size) == '0')
        {
            ++size;
        }
        else if (target.size() > size && target.at(size) >= '1' && target.at(size) <= '9')
        {
            ++size;
            while (target.size() > size && target.at(size) >= '0' && target.at(size) <= '9')
            {
                ++size;
            }
        }
        else
        {
            size = 0;
            return std::string_view();
        }

        // fraction
        if (target.size() > size && target.at(size) == '.')
        {
            ++size;
            if (target.size() > size && target.at(size) >= '0' && target.at(size) <= '9')
            {
                ++size;
                while (target.size() > size && target.at(size) >= '0' && target.at(size) <= '9')
                {
                    ++size;
                }
            }
            else
            {
                return std::string_view();
            }
        }

        // exponent
        if (target.size() > size && (target.at(size) == 'e' || target.at(size) == 'E'))
        {
            ++size;
            if (target.size() > size && (target.at(size) == '-' || target.at(size) == '+'))
            {
                ++size;
            }

            if (target.size() > size && target.at(size) >= '0' && target.at(size) <= '9')
            {
                ++size;
                while (target.size() > size && target.at(size) >= '0' && target.at(size) <= '9')
                {
                    ++size;
                }
            }
            else
            {
                return std::string_view();
            }
        }

        const std::string_view result = target.substr(0, size);
        target.remove_prefix(size);
        return result;
    }

    inline void strip_json_whitespace(std::string_view& json)
    {
        size_t i = 0;
        for (char character : json)
        {
            if (!(character == ' ' || character == '\t' || character == '\n' || character == '\r'))
            {
                break;
            }
            ++i;
        }
        json.remove_prefix(i);
    }

    inline std::vector<std::string_view> extract_json_array(std::string_view& json)
    {
        if (!strip_prefix("[", json))
        {
            // ERROR
        }

        strip_json_whitespace(json);

        if (strip_prefix("]", json))
        {
            return std::vector<std::string_view>();
        }

        std::vector<std::string_view> array_items;
        size_t levels = 0;
        for (size_t i = 0; i < json.length(); ++i)
        {
            if (levels == 0)
            {
                if (json.at(i) == ',')
                {
                    array_items.push_back(json.substr(0, i));
                    json.remove_prefix(i + 1);
                    i = 0;
                }
                else if (json.at(i) == ']')
                {
                    array_items.push_back(json.substr(0, i));
                    json.remove_prefix(i + 1);
                    break;
                }
            }

            if (json.at(i) == '[' || json.at(i) == '{')
            {
                ++levels;
            }
            else if (json.at(i) == ']' || json.at(i) == '}')
            {
                --levels;
            }
        }

        return array_items;
    }


    template <typename T>
    void fromJson(std::string_view& in, T& out)
    {
        using StrippedType = std::remove_cvref_t<T>;

        strip_json_whitespace(in);

        // JSON Boolean
        if constexpr (std::is_same_v<StrippedType, bool>)
        {
            if (strip_prefix("true", in))
            {
                out = true;
            }
            else if (strip_prefix("false", in))
            {
                out = false;
            }
            else
            {
                //ERROR
            }
        }
        // JSON Null
        else if constexpr (std::is_same_v<StrippedType, nullptr_t>)
        {
            if (strip_prefix("null", in))
            {
                out = nullptr;
            }
            else
            {
                //ERROR
            }
        }
        // JSON Number
        else if constexpr (std::is_integral_v<StrippedType>)
        {
            std::string_view number = extract_json_number(in);
            if (number.length() == 0)
            {
                // ERROR
            }

            std::from_chars_result result = std::from_chars(number.data(), number.data() + number.length(), out);
            if (result.ec != std::errc())
            {
                // ERROR
            }
        }
        else if constexpr (std::is_floating_point_v<StrippedType>)
        {
            std::string_view number = extract_json_number(in);
            if (number.length() == 0)
            {
                // ERROR
            }

            std::from_chars_result result = std::from_chars(number.data(), number.data() + number.length(), out);
            if (result.ec != std::errc())
            {
                // ERROR
            }
        }
        // JSON String
        else if constexpr (is_basic_string_v<StrippedType> || std::is_convertible_v<StrippedType, std::string>)
        {
            if (!strip_prefix("\"", in))
            {
                // ERROR
            }

            size_t str_size = in.find_first_of('"');
            out = in.substr(0, str_size);
            in.remove_prefix(str_size);

            if (!strip_prefix("\"", in))
            {
                // ERROR
            }
        }
        // JSON Array
        else if constexpr (std::is_bounded_array_v<StrippedType>)
        {
            std::vector<std::string_view> array_items = extract_json_array(in);

            if (array_items.size() != std::extent_v<StrippedType>)
            {
                // Error
            }

            for (size_t i = 0; i < std::extent_v<StrippedType>; ++i)
            {
                fromJson<std::remove_extent_t<StrippedType>>(array_items[i], out[i]);
            }
        }
        else if constexpr (is_std_array_v<StrippedType>)
        {
            std::vector<std::string_view> array_items = extract_json_array(in);

            if (array_items.size() != out.size())
            {
                // Error
            }

            for (size_t i = 0; i < out.size(); ++i)
            {
                fromJson<typename StrippedType::value_type>(array_items[i], out[i]);
            }
        }
        else if constexpr (is_std_vector_v<StrippedType>)
        {
            std::vector<std::string_view> array_items = extract_json_array(in);

            out.resize(array_items.size());

            for (size_t i = 0; i < StrippedType::size_type; ++i)
            {
                fromJson<typename StrippedType::value_type>(array_items[i], out[i]);
            }
        }
        // Reference
        else if constexpr (std::is_pointer_v<StrippedType>)
        {
            static_assert(false, "Not Implemented");
        }
        // JSON Object
        else if constexpr (refl::trait::is_reflectable_v<StrippedType>)
        {
            if (in.at(0) != '{')
            {
                throw bad_json();
            }
            in.remove_prefix(1);

            refl::util::for_each(refl::reflect(out).members, [&]<typename MD>(const MD member, size_t member_index)
            {
                if constexpr (refl::trait::is_field_v<MD>)
                {
                    using MT = MD::value_type;

                    if (member_index != 0)
                    {
                        if (in.at(0) != ',')
                        {
                            throw bad_json();
                        }
                        in.remove_prefix(1);
                    }

                    strip_json_whitespace(in);

                    if (in.at(0) != '"')
                    {
                        throw bad_json();
                    }
                    in.remove_prefix(1);

                    size_t str_size = in.find_first_of('"');
                    if (in.substr(0, str_size) != refl::descriptor::get_display_name(member))
                    {
                        throw json_schema_mismatch();
                    }
                    in.remove_prefix(str_size);

                    if (in.at(0) != '"')
                    {
                        throw bad_json();
                    }
                    in.remove_prefix(1);

                    strip_json_whitespace(in);

                    if (in.at(0) != ':')
                    {
                        throw bad_json();
                    }
                    in.remove_prefix(1);

                    strip_json_whitespace(in);

                    fromJson<MT>(in, member.get(out));

                    strip_json_whitespace(in);
                }
            });
            if (in.at(0) != '}')
            {
                throw bad_json();
            }
            in.remove_prefix(1);
        }
        // Unrepresentable
        else
        {
            static_assert(false, "Type cannot be serialized into JSON!");
        }

        strip_json_whitespace(in);
    }
}
