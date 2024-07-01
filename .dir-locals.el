((c-mode . ((eval .
		  ;; disable indentation for namespaces
		  (c-set-offset 'innamespace 0))
	    (c-basic-offset . 2)
	    (c-doc-comment-style . 'doxygen)))
 (c++-mode . ((eval .
		    ;; disable indentation for namespaces
		    (c-set-offset 'innamespace 0))
	      (c-basic-offset . 2)
	      (c-doc-comment-style . 'doxygen)))
 )
