
# glob all files only from <dir>
# matching any of <file_types> patterns
# return file paths in <result> variable

function(glob_files dir file_types result)
    set(files)

    foreach(file_type IN LISTS file_types)
        # set(files ${files} "${dir}/${file_types}")
        file(GLOB file_group "${dir}/${file_type}")
        list(APPEND files ${file_group})
    endforeach()

    # message("-|--> ${files}")
    set(${result} "${files}" PARENT_SCOPE)
endfunction()
