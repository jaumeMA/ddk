
function(remove_from_list in_list elems_to_remove)

	set(local_list ${${in_list}})
	list(LENGTH local_list list_size)

	if(NOT list_size EQUAL 0)
		foreach(elem ${${elems_to_remove}})
			list(FIND local_list ${elem} found_index)

			while(NOT found_index EQUAL -1)
				list(REMOVE_AT local_list ${found_index})
				list(FIND local_list ${elem} found_index)
			endwhile()
		endforeach(elem)
	endif()

	set(${in_list} ${local_list} PARENT_SCOPE)

endfunction()

function(append_suffix in_list suffix)

	foreach(elem ${${in_list}})
		list(APPEND local_list "${elem}${suffix}")
	endforeach(elem)

	set(${in_list} ${local_list} PARENT_SCOPE)

endfunction()