	Pentru rezolvarea temei am folosit o lista (text) in care am retinut textul si asupra careia am aplicat operatiile
ulterioare, o lista (del_nodes) in care am retinut caracterele sterse si inca o lista (saved_list) in care am pastrat
textul de la ultima salvare.
	 Functia read_text incepe prin cautarea pozitiei unde se va realiza inserarea textului prin intermediul functiei
search_pos. Daca stringul citit este diferit de "::i" atunci insereaza in lista fiecare caracter din stringul citit, 
altfel iese din functie. De asemenea, actualizeaza pozitia cursorului si sterge '\n' daca s-a inserat un rand nou la 
mijlocul unui rand existent. 
	Functia execute_commands citeste comanda si apeleaza functia corespunzatoare operatiei citite. Am folosit doi
vectori: numchar - retine numarul de caractere dupa fiecare inserare, c - retine pozitiile cursorului. Pentru inserarea 
unui nou caracter intr-o lista avem doua functii: Insert si insert. Insert inseraza la finalul listei si insert este 
folosita pentru inserarea la o anumita pozitie in text, la un anumit nod sau daca primeste NULL insereaza la finalul
listei. 
	Functia just_delete si backspace cauta nodul pe care se afla cursorul si sterge nodul urmator respectiv anterior
in functie de comanda apelata. Functia delete_word incepe prin apelarea functiei find_word. Daca aceasta intoarce un
pointer NULL delete_word returneaza 0, altfel se realizeaza stergerea caracter cu caracter a cuvantului din lista prin
apelarea repetata a functiei delnode. Functia delete_all apeleaza repetat functia delete_word cat timp find_word 
returneaza un pointer diferit de NULL (cuvantul inca se regaseste in text). Asemanator Replace sterge prima aparitie
de dupa cursor a cuvantului urmand sa insereze cuvantul urmator din string. Replace_all apeleaza Replace repetat cat 
timp find_word returneaza un pointer diferit de NULL.
	Functia Undo pentru insert sterge de la pozitia anterioara a cursorului vec[n] caractere pe care le insereaza
in del_nodes in cazul functiei de Redo. Pentru Undo delete se insereaza inapoi in lista ultimele caractere inserate in
del_nodes. Pentru a delimita cuvintele si liniile in del_nodes, dupa fiecare inserare am pus un terminator de linie.
Pentru Undo Replace am apelat Replace doar ca avand argumentele inversate. De asemenea, am folosit doua stive, undo
si redo, pentru a retine comenzile. La comanda Undo se executa inversa comenzii X, comanda ce se insereaza in redo si
este stearsa din undo. 
	Functia Redo apeleaza ultima comanda adaugata in stiva redo. Comanda este eliminata din redo si adaugata inapoi
in undo.
	La comanda save, se elibereaza lista saved_list si se copiaza in ea textul.
	Dupa comanda quit, se insereaza textul din saved_list in fisierul "editor.out" si se elibereaza memoria.  
