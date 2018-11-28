# PageRanking
Given Query string, search a large data-set and retrieve relevant documents


Files needed:

	main.c</br>
	implementation.c</br>
	suffix_tree.h</br>
	clean_data_cs.csv(the data set)</br>
  

Link to Data Set:</br>
https://drive.google.com/open?id=1n3jJB80O0_BC9Kd4YibQu24RIIy7Vez2</br>
(Store it in a Folder 'Data' after extracting)

Compilation Instruction:

	> gcc main.c implementation.c </br>
	> a.exe 

Please Follow on screen instructions after executing

Sample Output:

	Data Source Ready
	Enter Query String
	of

	How many documnets to Search?
	1

	Enter number of documents to be returned:
	1
	Run Case Sensitive Queries? (y/n) n

	First Occurence in Every document
	Doc 1 11

	********************************************************

	Search Results in order of relavence:

	Doc no: 1        -> ' the death of bunny munro' (1600)

	Tree construction time 0.001032
	Query Search Time 0.000133

	Time for the whole process : 0.002292

	Enter 0 to quit.
	Enter doc no, to view the document.

