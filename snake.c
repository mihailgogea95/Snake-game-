
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <sys/select.h>
#include <time.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	20
#define SELECT_EVENT    1
#define SELECT_NO_EVENT 0

/* curses.h este necesar pentru biblioteca grafică ncurses
 * ctype.h este necesar pentru funcția tolower - man tolower pentru detalii
 * Inițial, capul sarpelui se va afla pe ecran la coordonatele (INIT_ROW, INIT_COL) 
 */


// Structurile sarpe, obstacol, hrana sunt pentru a retine cordonatele sarpelui , obstacolului
// respectiv hranei pe coloane si linii. 
	typedef struct{ int linie ;
			int coloana ; }sarpe ;

	typedef struct{ int linie ;
			int coloana; }obstacol;

	typedef struct{ int linie;
			int coloana; }hrana;





// Aceasta functie chenar_meniu1 are rolul de a desena marginea primului meniu si de a introduce 
// propozitiile meniului .
// Cu ajutorul functiei attron incepe culoarea cu numarul 1 ce a fost definita in int main,
// functia move(rand , coloana) muta cursorul pe randul si coloana ceruta , functia addch 
// introduce caracterul special din bibloteca ncurses ( ACS_CKBOARD ) fiind un patratel 
// dar poate introduce orice caracter dorim.
// Functia attroff dezactiveaza culoarea .
	void chenar_meniu1(void) {  
					int i;
					
				        for(i = 0; i <= 49; i++) {    attron(COLOR_PAIR(1));
			
                        				          move(0 , i);
                                        			  addch(ACS_CKBOARD);
                                 	
								  move(18 , i);
                                     			          addch(ACS_CKBOARD);   }

                                        for(i = 0; i <= 18 ; i++) { 
								  move(i , 0);
                                                                  addch(ACS_CKBOARD);
                                                       
                                                                  move(i , 49);
                                                                  addch(ACS_CKBOARD);   }
				
		                       attron(COLOR_PAIR(2));

			       			move(3 , 16) ; addch(ACS_DIAMOND);
						move(3 , 17) ; addch(ACS_DIAMOND);
		                		move(3 , 27) ; addch(ACS_DIAMOND);
                                		move(3 , 28) ; addch(ACS_DIAMOND);
						move(1 ,  1) ; addch(ACS_CKBOARD);
						move(1 , 48) ; addch(ACS_CKBOARD);
						move(17 , 1) ; addch(ACS_CKBOARD);
						move(17 , 48); addch(ACS_CKBOARD);
			
				       attroff(COLOR_PAIR(2));
		

					        mvaddstr(3, 20, "SNAKE");
     					        mvaddstr(7, 2, "Pentru a juca cu obstacole, apasati tasta 1.");
					        mvaddstr(10, 2, "Pentru a juca fara obstacole, apasati tasta 2.");
					        mvaddstr(13, 2, "Pentru iesire, apasati tasta Q.");		

															}





// Functia paleta_culori defineste culorile de care am nevoie pe parcursul programului
// Functia init_pair (apartinand biblotecii ncurses)   are ca parametrii numarul culorii , 
// culoarea pe care o doresc si culoarea fundalului.
	void paleta_culori(void) {            

					        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
					        init_pair(2, COLOR_RED, COLOR_BLACK);
					        init_pair(3, COLOR_CYAN, COLOR_BLACK);
					        init_pair(4, COLOR_RED, COLOR_BLACK);
					        init_pair(5, COLOR_CYAN, COLOR_BLACK);   
						init_pair(6, COLOR_GREEN, COLOR_BLACK);
													  }






// Aceasta functie creeaza marginile jocului , folosind si culori in aceasta functie 
// Pentru fiecare linie si pentru fiecare coloana am facut am facut cate un for
// ca sa pot pune cu ajutorul functiei addch("simbol") patratelul de-a lungul 
// liniei si de-a lungul coloanei pentru a forma un chenar, deasemenea cu ajutorul
// functiei attron si attroff am activat si dezactivat culoarea.
	void chenar_joc(void) { 
				  int i;
				
			          for(i = 0 ; i <= 59 ; i++) {
								 move(7 , i) ;

                               	   				 attron(COLOR_PAIR(1));
                                 
				      					         addch(ACS_CKBOARD);

                                     				  move(29 , i);
                                       					         addch(ACS_CKBOARD); }
      

						                  move(7 , 0);     addch(ACS_CKBOARD);
							          move(29 , 0);    addch(ACS_CKBOARD);
       								  move(7 , 59);    addch(ACS_CKBOARD);
      								  move(29 , 59);   addch(ACS_CKBOARD);
       	
				  for(i = 8 ; i <= 28 ; i++) {
								  move(i , 0) ;
                                       						 addch(ACS_CKBOARD);
                                     				  move(i , 59);
                                     					         addch(ACS_CKBOARD);}
       								
								  attroff(COLOR_PAIR(1));
							
						        	}



// Aceasta functie sarpe_initial deseneaza sarpele initial , sarpele care apare la inceput 
// care este format din 5 patratele.

	void sarpe_initial(int row , int col) { 
							 attron(COLOR_PAIR(6));
        /* Se mută cursorul la poziția (row, col) */
       							 move(row, col);
        /* Se adaugă la poziția indicată de cursor caracterul dorit */
        					         addch(ACS_CKBOARD);

							 attroff(COLOR_PAIR(6));
							 attron(COLOR_PAIR(5));			 
				
       							 move(row, col-1);
  					                 addch(ACS_CKBOARD);

       							 move(row, col-2);
      						         addch(ACS_CKBOARD);
	
       							 move(row, col-3);
       							 addch(ACS_CKBOARD);

       							 move(row, col-4);
       							 addch(ACS_CKBOARD);

        						 attroff(COLOR_PAIR(5));           } 



// Aceasta functie introduce obstacolele .
// Functia for cu i de la 0 la 15 este pentru a adauga 15 obstacole in chenar,
// iar while pentru ca obstacolele sa intre in mijlocul chenarului, pentru a nu
// pica obstacolul peste sarpe .
// Functia rand() ( random ) este pentru a introduce numere aleatoriu, dar sunt 
// impartite la numarul 29 si 59 ca numarul respectiv sa fie in interiorul 
// chenarului.
	void introducere_obstacole(obstacol *bloc, sarpe *snake, int lungime )
				
				 { 
						
					int i , j ;

					for(i = 0; i <= 15 ; i++) {
                        		    
						    while(FOREVER) {
						
								 bloc[i].linie = rand() % 29 ;
                                        		         bloc[i].coloana = rand() % 59 ;

                                                	 if( bloc[i].linie <= 7 || bloc[i].coloana == 0 )
											 continue;

                                                	 for(j = 0; j < lungime; j++)
	
                                                         if(bloc[i].linie == snake[j].linie && bloc[i].coloana == snake[j].coloana)
                                                               				 continue;
                                                        	break; }
                              
					  move(bloc[i].linie , bloc[i].coloana);
                               		  addch(ACS_CKBOARD); }				} 
	





// Aceasta functie introduce random mancarea , la care sarpele creste cu un patratel de fiecare
// data cand mananca
// Functia while din interiorul functiei introducere_mancare este pentru ca mancarea sa nu cada
// peste sarpe , peste obstacole sau in afara chenarului(inclusiv chenarul).
	void introducere_mancare(hrana *mancare, obstacol *bloc, sarpe *snake, int lungime )
			
				 { 	int i , j ;
					
			                while(FOREVER) {   (*mancare).linie = rand() % 29 ;
                      				           (*mancare).coloana = rand() % 59 ;
                				       	
							  if((*mancare).linie <= 7 || (*mancare).coloana == 0 )
											 continue;

                             			          for(j=0; j<lungime; j++)
                                     
					         	  if((*mancare).linie == snake[j].linie && (*mancare).coloana == snake[j].coloana)
                                                                                         continue;
                              
							  for(i = 0; i < 15; i++)
	
                                      			  if((*mancare).linie == bloc[i].linie && (*mancare).coloana == bloc[i].coloana)
                                                         			         continue;
                                              		  break; }
                          
						
	                             attron(COLOR_PAIR(4));
                            	    
				     move((*mancare).linie , (*mancare).coloana);
                                     addch(ACS_DIAMOND);
                                  
				     attroff(COLOR_PAIR(4));                            }






int main(void)
{

	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int i, m=0, logic;
	int lungime=5;
	int nfds , sel ;
	fd_set read_descriptors;
	char c='d', C;
	struct timeval timeout;

	sarpe snake[100];
	obstacol bloc[100];
	hrana mancare;


	for(i = 0; i <= 15 ; i++) {
					 bloc[i].linie = 0 ;
				         bloc[i].coloana = 0;       } 


	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr();
	
	/* getmaxyx - este un macro, nu o funcție, așă că nu avem adresă la parametri */
	/* Se va reține în nrows și ncols numărul maxim de linii și coloane */
	/* getmaxyx(wnd, nrows, ncols); */
	wresize(wnd, 30 , 60);

	/* Se șterge ecranul */
	clear();
	
	/* Se inhibă afișarea caracterelor introduse de la tastatură */
	noecho();
	
	/* Caracterele introduse sunt citite imediat - fără 'buffering' */
	cbreak();
	
	// Aceasta functie este pentru a permite introducerea culorilor in program.
	start_color();	
	
	// introduc culorile in main
	paleta_culori();

	/* Se ascunde cursorul */	
	curs_set(0);
	
	// Introduc meniul principal 
	chenar_meniu1();
	
	
	refresh();	

	//introduc o tasta de la tastatura
	c = getchar();

	if(c == 'q') logic = 0;
		else if(c == '1' ) logic = 1;
			else logic = 2 ; 	 	



	if(logic != 0  ) { 
		
	  clear();

        
// Am initializat tasta la inceput cu 'd' pentru ca sarpele sa inceapa sa mearga in dreapta 
// cand playerul porneste jocul .	
// Timeout.tv_sec = 1  este pentru ca sarpele sa astepte timp de o secunda inainte ca el sa 
// porneasca fix cand se intra in joc. 
	c = 'd';
	nfds = 1 ; 
	FD_ZERO(&read_descriptors);
	FD_SET(0, &read_descriptors);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0 ;


// Cu ajutorul functiei mvaddstr( apartinand biblotecii ncurses ) am adaugat propozitiile
// meniului de deasupra chenarului cand se intra in joc .
	/* Se va afișa un mesaj la poziția formată din primii doi parametri - (par1, par2) */	
	mvaddstr(0, 1, "Puteti sa mutati sarpele folosind tastele:");
	mvaddstr(1, 2, "A - stanga");
	mvaddstr(2, 2, "D - dreapta");
	mvaddstr(3, 2, "W - sus");
	mvaddstr(4, 2, "S - jos");
	mvaddstr(5, 1, "Pentru iesire, apasati tasta Q.");

	attroff(COLOR_PAIR(2));
		
	
	//Chenarul jocului
	chenar_joc();
 
	// Aceasta functie introduce sarpele in chenar la inceput, fiind format din 5
	// patratele.
	sarpe_initial(row,col);


	/* Se reflectă schimbările pe ecran */
	refresh();

	for(i = 0; i < lungime ; i++) {  snake[i].linie = row ;		    
			   		 snake[i].coloana = col-i ;
				       } 

	srand(time(NULL));
	

	// Playerul daca va testa 1 , jocul va incepe cu obstacole.
	if(logic == 1 )
	    introducere_obstacole(bloc,snake,lungime);
	

	introducere_mancare(&mancare , bloc , snake , lungime);

	
	refresh();											
	
	
	/* Rămânem în while până când se primește tasta q */
	
// Functia mvwprintw cu parametrii wnd(window), linia , coloana , mesaj si numarul dorit 
// este pentru a afisa scorul playerului , dupa cum bine se vede dedesubtul lui este un
// refresh() pentru a da un refresh ecranului ca scorul sa se schimbe de fiecare data 
// cand sarpele mai mananca un patratel.
	while (FOREVER) { 
		
		  attron(COLOR_PAIR(1));
		            
			    mvwprintw(wnd, 6, 45, "SCOR: %d", lungime-5);
	
                  attroff(COLOR_PAIR(1));
		
		   	    refresh();
		
			    sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
		
			  if( sel == SELECT_EVENT ) {
				
				C = getchar();
				C = tolower(C);

// Aceste if este pentru ca playerul cand apasa alta tasta decat 'wasdq' sa nu se
// intample nimic , iar programul asteapta sa se apese potrivit.
				if( strchr("wasdq",C) == 0) continue;	
					else c = C;
	
				if (tolower(c) == 'q') {
								break;
										}}
	
		/* Se determină noua poziție, în funcție de tasta apăsată
		 * Nu putem depași nrows și ncols, sau linia 0/coloana 0.
		 */
		
// Aceste switch este pentru ca sarpele sa se miste in directia dorita
// a in stanga, d in dreapta, w in sus si s in jos.
		switch (tolower(c)) {
		case 'a':
			new_row = row;
			if (col > 0)
				new_col = col - 1;
			if (new_col == 0 ) { m = 1;}
			break;
		
		case 'd':
			new_row = row;
			if (col + 1 < 60)
				new_col = col + 1;
			if(new_col == 59 ) { m = 1; }
			break;
		
		case 'w':
			if (row > 0)
				new_row = row - 1;
			new_col = col;
			if(new_row == 7 ) { m = 1;}
			break ;

		case 's':
			if (row + 1 < 30)
				new_row = row + 1;
			new_col = col;
			if(new_row == 29 ) { m = 1;}
			break;
		}
		
		 	for(i = 0; i <= 15 ; i++) {
					
					 if(  bloc[i].linie == new_row && bloc[i].coloana == new_col)
						m = 1 ; }
			for(i=0; i<lungime; i++) {
					 if(  snake[i].linie == new_row && snake[i].coloana == new_col)
                                                m = 1 ;}
						
			
			move(snake[lungime-1].linie, snake[lungime-1].coloana);

                        addch(' ');

			
			for(i = lungime-1; i > 0 ; i--) {
							
						 snake[i].linie = snake[i-1].linie ;
					         snake[i].coloana = snake[i-1].coloana ; }
			
					   	 snake[0].linie = new_row;		
			                         snake[0].coloana = new_col ; 		
			
			attron(COLOR_PAIR(6));

						move(snake[0].linie, snake[0].coloana);
						addch(ACS_CKBOARD);
			attroff(COLOR_PAIR(6));

			attron(COLOR_PAIR(5));
			
			for(i=1; i<lungime; i++) { 
			
						 move(snake[i].linie, snake[i].coloana);
					         addch(ACS_CKBOARD); }
			                 
			attroff(COLOR_PAIR(5));
			

		if(m == 1) {   break ;	}	

		if(new_row == mancare.linie && new_col == mancare.coloana ) { 
						 lungime++;
			    
						 introducere_mancare(&mancare,bloc,snake,lungime);    }

		
		attron(COLOR_PAIR(6));
		/* Se adaugă '*' în noua poziție */
			move(new_row, new_col);
			addch(ACS_CKBOARD);
	
		attroff(COLOR_PAIR(6));		

	//Se va da refresh la ecran 
	//timeout.tv_usec este timpul reprezentat in milisecunde , iar cu timpul cand mananca
	// cate un patratel de hrana si lungimea sarpelui creste , viteza acestua va creste.
		refresh();
		FD_SET(0, &read_descriptors);
		timeout.tv_sec = 0; 
		timeout.tv_usec = 750000-(lungime-5)*40000;
		row = new_row;
		col = new_col;

	}}

	if(tolower(c) != 'q') 


// Aceste while este pentru a sterge ecranul atunci cand sarpele se musca singur
// sau se loveste de perete , si va aparea mesajul GAME OVER , scorul pe care 
// l-a facut playerul si posibilitatea de a iesi din joc cu ajutorul tastei Q.
	while(FOREVER) {    clear();
			
		         attron(COLOR_PAIR(1));
			   
			    move(5, 12); addch(ACS_DIAMOND);
			    move(5, 27); addch(ACS_DIAMOND);
			    mvaddstr(5, 15 , "GAME OVER !");
		
			 attroff(COLOR_PAIR(1));
		
			 if(lungime-5 != 1)
				 mvwprintw(wnd, 12, 8, "Scorul tau : %d puncte.", lungime-5);
			   else  mvwprintw(wnd, 12, 8, "Scorul tau : %d punct." , lungime-5);
		
		       	    mvaddstr(14, 8 , "Pentru iesire, apasa tasta Q." );
		
			refresh();
		
			 c=getchar();
		
			if(tolower(c) == 'q' ) break; 
							 }
	/* Se închide fereastra ncurses */	
	endwin();



	return 0;

			}
