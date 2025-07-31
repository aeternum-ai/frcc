# Poor man's FetchContent
function(add_subdirectory_if_no_target dir target)
  if (NOT TARGET ${target})
    add_subdirectory(${dir})
  endif()
endfunction()