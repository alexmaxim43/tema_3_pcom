Maxim Alexandru Gabriel 324CC


	Am pornit de la scheletul de laborator si am adaugat in request.c
la fiecare functie, parametrul "authorization" in care voi retine cheia
de autorizare fara de care user-ul nu are acces la biblioteca.

	In request.c am implementat functia "compute_delete_request"
care este similara cu functia get, doar ca fara parametrii query_params
si cookie.

	In client.cpp am un while in care deschid conexiunea cu 
serverul, apoi citesc fiecare comanda, acesta ruleaza cat timp variabila
"ok" este egala cu 1. Iar apoi, am un "if" pentru fiecare comanda:


Comanda register => se citeste de la tastatura un username si o parola
pe care le salvam intr-un fisier de tip json. Trimit un request de tip
POST fara cheie de autorizare si cookies, in care trimit server-ului 
fisierul json, iar apoi printez mesajul primit de la server.


Comanda login => este identica cu functia register doar ca in plus
caut cookie-ul dupa "connect.sid=" si il salvez pentru task-urile
urmatoare.


Comanda enter_library => efectuam un request de tip GET in care trimitem
la server cookie-ul, daca acesta nu este gol, inseamna ca login-ul s-a
efectual cu succes, deci putem salva jwt-ul care va reprezenta cheia de
autorizare a user-ului.


Comanda get_books => efectueaza un request de tip GET in care trimitem
cheia de autorizare, salvata in variabila "token" in cadrul functiei
"enter_library", iar apoi printam mesajul primit de la server.


Comanda get_book => este similara cu functia "get_books" doar ca inainte
sa trimitem la server request-ul, user-ul trebuie sa introduca de la
tastatura id-ul cartii despre care doreste sa vada detaliile, id
pe care il vom adauga in noul url.


Comanda add_book => citim intr-un json toate datele unei carti (title,
author, genre, publisher, number of pages), date pe care le trimitem
server-ului print-un request de tip POST, alaturi de cheia de autentificare.


Comanda delete_book => este similara cu functia "get_book", citim id-ul
cartii pe care user-ul doreste sa o elimine si il adaugam in noul url 
doar ca request-ul pe care il trimitem la server este de tip DELETE.


Comanda logout => efectuam un request de tip GET in care trimitem la 
server cookie-ul salvat la login, iar apoi resetam respectivul cookie
si token-ul.


Comanda exit => setam ok = 0 si dam un break pentru a iesi din while.


	M-am folosit de nlohmann in client.cpp pentru a formata mai usor
datele trimise catre server si ca sa le trimit intr-un mod structurat.
Am mai folosit functia dump() din libraria de json pt a face din json
un string, aveam nevoie de string la request-uri. 