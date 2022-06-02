#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

const string moviesFileName = "movies.csv";
const string seatsFileName = "seats.csv";
const string servicesFileName = "services.csv";

//structs
struct DateTime{
	int day;
	int month;
	int year;
	int hour;
	int minute;
};
struct Seat{ // '1A', '2B'
	char row;
	int col;
	int price;//seat price, to be implemented in future
	string terminal;
	char booked;
	string movie;
};
struct Movie{
	string title;
	DateTime startTime;
	int length;
	string category; //'Horror' 'Thrill' 'Action'
	string ageRestriction;
	int availableTickets;
};
struct Terminal{
	string type;
	int price;
};
struct Service{
	string name; // snacks, headsets, 
	int price;
};
struct Ticket{
	int id;
	int totalPrice;
	Terminal term;
	Seat seat;
};
struct User{
	int id;
	string name;
	string password;
};

//initilizing the date (global)
int moviesIndex = -1, usersIndex = -1, seatsIndex=-1, searvicesIndex=-1;
Movie movies[1000];
User users[100];
Seat seats[100]; 
Service services[100];
Terminal terminals[2];
//..
//.. 

void initMovies();
void initSeats();
void initServices();
void initTerminals();

//helper functions
void printMovie(Movie m);
void printSeat(Seat s);
string dateToString(DateTime d);
int generateRandomId();
void calculateTax(double* totalPrice);
bool isSeatAvailable(char row,int col,string terminal);
int getSeatPrice(char row,int col,string terminal);
int getSeatIndex(char row,int col,string terminal,string movieName);

//menu functions
void welcome();
void showMovies(); //with the option to filter movies based on age
void showOffers();
void showServices();
void showTerminals();

//tickets functions
void buyTickets(); //number of tickets and total price

//booking functions
void showTicketTerminals();
void showSeats();
bool bookSeat(string seatId);


//registeration functions
bool isMember(int id);
bool isMember(string email);
int reg(string email);

//test
void saveMoviesData();
void saveSeatsData();
void saveUsersData();

int main(){

	welcome();
	
	//main menu
	/*char ans;
	cout<< "Welcome, are you a rigested member? (Y,n): ";
	cin>>ans;
	bool isRigesred = false;
	switch(ans){
		case 'y':
		isRigesred = true;
		break;
		case 'Y':
		isRigesred = true;
		break;
	}
	cin.ignore();
	if(!isRigesred){
		string name;
		cout<<"Please rigester first to buy tickets. Enter your name: ";
		getline(cin,name);
		cout<<"Your name: "<<name<<endl;
	}*/
	
	initMovies();
	initSeats();
	initServices();

	
	int intnum;
	char ans;
	do {
			
				int selectedMovie,selectedSeat;
				showMovies();
				cout<<"Please select a movie to buy tickets or type 0 to quit."<<endl;
				double totalPrice = 0.0;
				string selectedMovieStr;
				cin>>intnum;
				if(intnum > 0 && intnum <= moviesIndex+1){
					if(movies[(intnum-1)].availableTickets > 0){
						selectedMovie = intnum-1;
						selectedMovieStr = movies[selectedMovie].title;
						cout<<"Please select a terminal:"<<endl;
						cout<<"a. VIP"<<endl;
						cout<<"b. Standard"<<endl;
						cin>>ans;
						cout<<"The available seats:"<<endl;
						cout<<"-*--*--*--*--*--*--*--*--*-"<<endl;
						int col;
						char row;
						if(ans == 'a'){
							//show seats
							int lastSeat = 0;
							for(int i=0;i<=seatsIndex;i++){
								if(seats[i].terminal == "vip" && seats[i].movie == movies[selectedMovie].title){
									//cout<<(++lastSeat)<<".";
									printSeat(seats[i]);
									cout<<"-*--*--*--*--*--*--*--*--*-"<<endl;
									lastSeat++;
								}
							}

							cout<<"Your selection (e.g A1): ";
							cin>>row>>col;
							if(isSeatAvailable(row,col,"vip")){
								
									selectedSeat = getSeatIndex(row,col,"vip",selectedMovieStr);
									totalPrice += seats[selectedSeat].price;
									cout<<"Would like to have extra services:"<<endl;
									for(int i=0; i<=searvicesIndex;i++){
										cout<<(i+1)<<". ";
										cout<<services[i].name<<", price: +SAR"<<services[i].price<<endl;
									}
									cout<<"0. None"<<endl;
									cout<<"Your selection: ";
									cin>>intnum;
									if(intnum != 0){
										totalPrice += services[(intnum-1)].price;
										calculateTax(&totalPrice);
									}

									cout<<"Your total price is: "<<totalPrice<<endl;
									cout<<"Confirm booking?(y/n): ";
									cin>>ans;
									if(ans == 'y'){
										seats[selectedSeat].booked = '0';
										movies[selectedMovie].availableTickets -= 1;
										cout<<"You have been booked successfully."<<endl;
									}
								
								}else{
									cout<<"Sorry, this seat is not available!!"<<endl;
								}
						}else if(ans == 'b'){
							//show seats
							int lastSeat = 0;
							for(int i=0;i<=seatsIndex;i++){
								if(seats[i].terminal == "standard" && seats[i].movie == movies[selectedMovie].title){
									//cout<<(++lastSeat)<<".";
									printSeat(seats[i]);
									cout<<"-*--*--*--*--*--*--*--*--*-"<<endl;
									lastSeat++;
								}
							}

							cout<<"Your selection (e.g. A1): ";
							cin>>row>>col;
							if(isSeatAvailable(row,col,"standard")){
									selectedSeat = getSeatIndex(row,col,"standard",selectedMovieStr);
									cout<<"Index: "<<selectedSeat<<endl;
									totalPrice += seats[selectedSeat].price;
									calculateTax(&totalPrice);
									cout<<"Your total price is: "<<totalPrice<<endl;
									cout<<"Confirm booking?(y/n): ";
									cin>>ans;
									if(ans == 'y'){
										seats[selectedSeat].booked = '0';
										movies[selectedMovie].availableTickets -= 1;
										cout<<"You have been booked successfully."<<endl;
									}
							}else{
								cout<<"Your selection is out of range.. please try again."<<endl;
								break;
							}


						}else{
							cout<<"Pleas selected an invalid option!!"<<endl;
						}
					}else{
						cout<<"Sorry, there are not available tickets for this movie!!"<<endl;
					}

					saveMoviesData();
					saveSeatsData();
					
				}
			

	}while(intnum > 0);

	cout<<"Good Bye. Visit us again !"<<endl;
	
	

	//showMovies();
	//saveMoviesData();
	return 0;
}

void initMovies(){ //read from csv file/ text file

	string fname = moviesFileName;
 
	string line, word;
 
	fstream file (fname, ios::in);
	Movie temp;
	if(file.is_open())
	{
		while(getline(file, line))
		{
 
 			//we learned this from a tutorial from the internet
 			//source: 
			stringstream str(line);
			
			getline(str, word, ','); 
			temp.title = word;

			getline(str, word, ','); 
			temp.category = word;

			getline(str, word, ',');
			temp.length = stoi(word); //stoi to convert from string to intiger

			getline(str, word, ',');
			temp.ageRestriction = word;

			getline(str, word, ',');//date is a bit tricky
			stringstream dstr(word); //word --> 4/4/2022 10:15
			
			//date part
			getline(dstr,word,' ');
			stringstream datestr(word);

			getline(datestr,word,'/');
			temp.startTime.day = stoi(word); //stoi to convert from string to intiger
			getline(datestr,word,'/');
			temp.startTime.month = stoi(word);
			getline(datestr,word,'/');
			temp.startTime.year = stoi(word);

			//time part
			getline(dstr,word,' ');
			stringstream timestr(word);

			getline(timestr,word,':');
			temp.startTime.hour = stoi(word);
			getline(timestr,word,':');
			temp.startTime.minute = stoi(word);

			getline(str, word, ',');//
			temp.availableTickets = stoi(word);

			// add struct to array
			++moviesIndex;
			movies[moviesIndex] = temp;

		}
	}
	else
		cout<<"Could not open the file\n";
		
}


void initSeats(){

	string fname = seatsFileName;
 
	string line, word;
 
	fstream file (fname, ios::in);
	Seat temp;
	if(file.is_open())
	{
		while(getline(file, line))
		{
 			
			stringstream str(line);
			
			getline(str, word, ','); 
			temp.row = word[0];

			getline(str, word, ','); 
			temp.col = stoi(word);

			getline(str, word, ',');
			temp.price = stoi(word);

			getline(str, word, ',');
			temp.terminal = word;

			getline(str, word, ',');
			temp.booked = word[0];

			getline(str, word, ',');
			temp.movie = word;
			

			
			// add struct to array
			++seatsIndex;
			seats[seatsIndex] = temp;

		}
	}
	else
		cout<<"Could not open the file\n";

}

void initServices(){

	string fname = "services.csv";
 
	string line, word;
 
	fstream file (fname, ios::in);
	Service temp;
	if(file.is_open())
	{
		while(getline(file, line))
		{
			stringstream str(line);
			
			getline(str, word, ','); 
			temp.name = word;

			getline(str, word, ','); 
			temp.price = stoi(word);

			
			// add struct to array
			++searvicesIndex;
			services[searvicesIndex] = temp;

		}
	}
	else
		cout<<"Could not open the file\n";

}

void getUsers(){ //read from csv file/ text file

	string fname = "users.csv";
 
	string line, word;
 
	fstream file (fname, ios::in);
	User temp;
	if(file.is_open())
	{
		while(getline(file, line))
		{
 
			stringstream str(line);
			
			getline(str, word, ','); 
			temp.id = stoi(word);

			getline(str, word, ','); 
			temp.name = word;

			getline(str, word, ',');
			temp.password = word;

			
			// add struct to array
			++moviesIndex;
			users[usersIndex] = temp;

		}
	}
	else
		cout<<"Could not open the file\n";
		
}

void saveMoviesData()
{
    // file pointer
    fstream fout;
  
    // opens the csv file and overwrite it.
    fout.open(moviesFileName, ios::out);
  
        // Insert the data to file
	for(int i=0;i<= moviesIndex;i++){
		fout << movies[i].title << ","
			 << movies[i].category << ","
			 << movies[i].length << ","
			 << movies[i].ageRestriction << ","
			 << dateToString(movies[i].startTime) << ","
			 << movies[i].availableTickets << "\n";
	}
    
}

void saveSeatsData()
{
    // file pointer
    fstream fout;
  
    // opens the csv file and overwrite it.
    fout.open(seatsFileName, ios::out);
  
        // Insert the data to file
	for(int i=0;i<= seatsIndex;i++){
		fout << seats[i].row << ","
			 << seats[i].col << ","
			 << seats[i].price << ","
			 << seats[i].terminal << ","
			 << seats[i].booked << ","
			 << seats[i].movie << "\n";
	}

	fout.close();

    
}

void saveUsersData()
{
    // file pointer
    fstream fout;
  
    // opens the csv file and overwrite it.
    fout.open("users.csv", ios::out);
  
        // Insert the data to file
	for(int i=0;i<= usersIndex;i++){
		fout << users[i].id << ", "
			 << users[i].name << ", "
			 << users[i].password << "\n";
	}

	//fout.close();
    
}


void printMovie(Movie m){
	cout<<m.title<<endl<<m.category<<endl<<dateToString(m.startTime)<<endl<<"Available Tickets:"<<m.availableTickets<<endl;
}
void printSeat(Seat s){
	cout<<s.row<<s.col<<"\n"<<"Price: SAR"<<s.price<<endl;
	if(s.booked == '1')
		cout<<"Booked: No";
	else
		cout<<"Booked: Yes";
	cout<<endl;
}

bool isSeatAvailable(char row,int col,string terminal){
	for(int i=0;i<=seatsIndex;i++){
		if(seats[i].row == row && seats[i].col == col && seats[i].terminal == terminal){
			if(seats[i].booked == '1')
				return true;
			else
				return false;
		}
	}
	return false;
}

int getSeatPrice(char row,int col,string terminal){
	for(int i=0;i<=seatsIndex;i++){
		if(seats[i].row == row && seats[i].col == col && seats[i].terminal == terminal)
			return seats[i].price;
	}
	return 0;
}

int getSeatIndex(char row,int col,string terminal,string movieName){
	for(int i=0;i<=seatsIndex;i++){
		if(seats[i].row == row && seats[i].col == col && seats[i].terminal == terminal && seats[i].movie == movieName)
			return i;
	}
	return -1;
}

string dateToString(DateTime d){
	return to_string(d.day)+"/"+to_string(d.month)+"/"+to_string(d.year)+" "+to_string(d.hour)+":"+to_string(d.minute);
}

void showMovies(){
	int count = sizeof(movies)/sizeof(movies[0]);
	cout<<"---------------------"<<endl;
	for (size_t i = 0; i <= moviesIndex; i++)
	{
			cout<<(i+1)<<"."<<endl;
			printMovie(movies[i]);
			cout<<"---------------------"<<endl;
	}
	
}

void calculateTax(double* totalPrice){
	*totalPrice = *totalPrice * 1.15;
}

void welcome(){
	cout << "*******************************" << "\n"
		 << "*          Welcome to         *" << "\n"
		 << "*         A.L.M Cinema        *" << "\n"
		 << "*******************************" << "\n";
}