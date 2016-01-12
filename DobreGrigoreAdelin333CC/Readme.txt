Tema 2 EGC
Autor: Dobre Grigore Adelin 333CC

Detalii de implementare:
In rezolvarea temei, am plecat de la scheletul de cod oferit de laboratorul 5.
Scena este realizata integral din cuburi, desenate cu ajutorul functiei "myLoadCube_as_Segments" preluata din laboratorul 4.
In aplicatie, jucatorul este desenat cu verde, si trebuie sa ajunga la destinatie, cubul desenat cu rosu.

Functia gravityTest(), testeaza daca sub pozitia curenta a jucatorului, se afla vreun cub la distanta mai mare de o latura fata de centrul cubului. In cazul in care spre exemplu, centrul cubului se afla la inaltimea de 60, si nu mai este nicun cub sub el, atunci jucatorul va reveni la nivelul 0. Pentru tratarea coliziunilor, compar intotdeauna centrurile cuburilor.
Functia "spacebarpressed()" este apelata la fiecare frame si se executa in cazul in care este apasata tasta space. Se realizeaza o serie de translatii care sa simuleze cat mai bine o saritura dintr-un joc video. 

Variabila orientation se modifica ori de cate ori exista o schimbare de perspectiva a camerei TPS. Functia computeTranslation() se ocupa de tratarea coliziunilor pentru fiecare din cele 4 cazuri ale schimbarii de perspectiva.Astfel, miscarile stanga - dreapta ale jucatorilor vor fi restrictionate in cazul in care in calea lor apare un alt cub.

Pentru determinarea sensului in care se executa saritura, trebuie apasat inainte tasta 'a' sau 'd', urmata de tasta space. Ultima tasta apasata dintre 'a' si 'd'va determina sensul sariturii, urmata fiind bineinteles si de tasta 'space'.De asemenea, in notifyKeyPressed, la apasarea tastei space, se testeaza si posibilitatea realizarii sariturii in toate cele 4 cazuri de orientari.
Astfel, daca fata de pozitia curenta a jucatorului, se afla un cub imediat deasupra lui(sau deasupra lui,in stanga sau in dreapta) atunci saritura nu se a putea realiza.

Implementarea camerei FPS: 
Camera FPS priveste din perspectiva jucatorului, astefl ca imaginea proiectata este cea pe care o are jucatorul in functie de deirectia ultimei miscari. Camera fps isi modifica cadrul surpinsa la fiecare translatie a jucatorului, la fiecare saritura sau schimbare de perspectiva a camerei TPS.  