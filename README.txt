

A.	The .so file is compiled with clang, but in school servers it uses gcc. 
 	I already ask another team to sent me the version compiled with g++.】

B.	I will update it when I get the new shared library file ASAP

C.	It works perfect in macOS terminal, if I cannot reach another team, I am able to 
	demo my application, my email is lxyamerica@gmail.com

D.	How it works

		1. How to compile
			1. cd to correct direction
			2. g++ -std=c++11 *.cpp -L. team1lib.so

		2. How to run
			1. ./a.out
			2. MAKE SURE TO MAXIMIZE TERMINAL !!!!!

E.	Other Issues
	
	For performance issue, I only take first 20000 line of original file,
	it will increase the loading speed, but also will cause some issues:
		
		1.	For example, If you want to analysis for user name “rob”, may be it will
			print error message “NOT FOUND”. This is because the TIPS, REVIEW, 
			such information regard this user may be store in line 20001 ~ 99999.
			So I suggest you use common name, such as “bob”, “john”.


		2.	regarding business analysis, I changed few business_id in Tip_data, 
			review_data, checkin_data. This is also because such information regard 
			this business may be store in line 20001 ~ 99999.
			Here are good example for grading purpose : pizza pizza, evie Lou, move for less

		3. 	Yelp user data set has some user name “NOT”, “COOL”,”AND”, etc. This is reserve word
			in their library, so I rename them to “NOT1, “COOL1” etc,

