local DependencyFetcher = {}

function DependencyFetcher.FetchRepo(url, targetDir, treeName, commitID)
    if not os.isdir(targetDir) then
        print("[DependencyFetcher] Cloning " .. url .. " to " .. targetDir)
        
        -- Base clone command that we'll append our needed data onto if we have a specific branch we want to pull.
        local cloneCmd = "git clone --depth 1"
        
        -- If a tree name is provided, we'll add the --branch option to the clone command.
        -- This works for branches and should work for tags as well.
        if treeName then
            print("[DependencyFetcher] Targeting tree: " .. treeName)
            cloneCmd = cloneCmd .. " --branch " .. treeName
        end
        
        -- Append the URL onto it.
        cloneCmd = cloneCmd .. " " .. url .. " " .. targetDir
        
        local result = os.execute(cloneCmd)
        if not result then
            error("[DependencyFetcher] Failed to clone repository: " .. url .. (treeName and (" (tree: " .. treeName .. ")") or ""))
        end

        if commitID then
            print("[DependencyFetcher] Checking out specific commit: " .. commitID)
            -- Using --depth 1 could possibly cause issues for very old commits. It should work for most
            -- But modify it if there is trouble 
            local fetchCmd = string.format("git -C %s fetch --depth 1 origin %s", targetDir, commitID)
            local fetchResult = os.execute(fetchCmd)

            local checkoutCmd = string.format("git -C %s checkout %s", targetDir, commitID)
            local checkoutResult = os.execute(checkoutCmd)
            if not checkoutResult then
                error("[DependencyFetcher] Failed to checkout commit: " .. commitID)
            end
        end

    else
        -- In the future, support for existing directories and overwriting could be nice.
        print("[DependencyFetcher] [Warning] Found existing: " .. targetDir .. " (no new files were added/changed)")
    end

end

return DependencyFetcher