#include <iostream>
#include <string>
#include <unistd.h>


// Helper Functions
void mySleep(int secs){
	usleep(1000000 * secs);
}




class Game
{

private:

	int player1 = 0;
	int player2 = 1;
	int current_player = 0;
	std::string* nameArr = new std::string[2];
	std::string* symbolArr = new std::string[2];
	int** board = new int*[3];
	const int margin = 3;

	const std::string CORNER = ".";
	const std::string UPRIGHT = "|";
	const std::string FLAT = "=";

	bool ERROR = false;

	int DIR[8][2] = { {0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {-1,-1}, {1,-1} };


public:

	Game(){}
	

	void start(){
		for (int i = 0; i < 3; ++i){
			board[i] = new int[3];
		}
		std::cout << "Please enter player1 name: ";
		std::cin >> nameArr[0];
		std::cout << "Please enter player2 name: ";
		std::cin >> nameArr[1];

		std::string player1sym;
		std::cout << "Does " << nameArr[0] << " want to be 'x' or 'o'? ";
		std::cin >> player1sym;
		set_symbols(player1sym);

		initiate_board();
		std::cin.ignore(1000, '\n');
		new_play();
	}


	void initiate_board(){
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 3; ++j){
				board[i][j] = -1;
			}
		}
	}


	void set_symbols(std::string sym){
		symbolArr[0] = (sym == "x") ? "x" : "o";
		symbolArr[1] = (sym == "x") ? "o" : "x";
	}




	void new_play(){
		print_board();
		
		std::cout << nameArr[current_player] << ", make a move:" << std::endl;
		std::string stringCoord;

		
		std::getline(std::cin, stringCoord);
		
		int x, y;
		
		x = stringCoord[0] - '0'; // converts char to int -___-
		y = stringCoord[1] - '0'; // ^

		errorCheckCoords(x, y);

		if (!ERROR){
			board[(x-1)][(y-1)] = current_player;

			// Game has ENDED
			if (game_over()){
				print_board();
				print_final_message();
				return;

			} else if (board_full()){
				// DRAW
				print_board();
				print_draw_message();
				return;
			}

			switch_player();
		}
		
		ERROR = false;
		new_play();
	}
	


	void errorCheckCoords(int x, int y){

		if (!in_range(x-1, y-1)){
			std::cout << "Invalid coordinates. Try again." << std::endl;
			ERROR = true;

		} else if (board[x-1][y-1] != -1){
			std::cout << "Cant play there! Try again." << std::endl;
			ERROR = true;
		}

		if (ERROR){
			mySleep(2);
		}

	}



	void print_board(){
		myclear();

		print_unit("3");
		print_unit("2");
		print_unit("1");

		print_border(); // print closing bottom border
		print_column_numbers();
		std::cout << "\n";
		
	}

	void print_unit(std::string line_number){
		// prints everything except for the bottom closing border
		print_border();
		print_middleman();
		print_XO_row(line_number);
		print_middleman();
	}


	void print_border(){
		print_margin();
		for (int i = 0; i < 19; ++i){
			if (i == 0 || i == 18){
				std::cout << CORNER;
			} else {
				std::cout << FLAT;
			}
			
		}
		std::cout << "\n";
	}


	void print_middleman(){
		// VERY hardcody, but it works for now
		print_margin();
		std::cout << UPRIGHT << "     "  << UPRIGHT << "     " << UPRIGHT << "     " << UPRIGHT;
		std::cout << "\n";
	}


	void print_XO_row(std::string ln){
		print_margin(ln);

		std::string a;
		std::string b;
		std::string c;

		//std::cout << "HEY";
		int spot1 = board[0][stoi(ln) - 1];
		int spot2 = board[1][stoi(ln) - 1];
		int spot3 = board[2][stoi(ln) - 1];

		
		a = (spot1 != -1) ? symbolArr[spot1] : " ";
		b = (spot2 != -1) ? symbolArr[spot2] : " ";
		c = (spot3 != -1) ? symbolArr[spot3] : " ";

		
		std::cout << UPRIGHT << "  " << a << "  " << UPRIGHT << "  " << b << "  " << UPRIGHT << "  " << c << "  " << UPRIGHT;
		std::cout << "\n";
	}


	void print_margin(std::string line_number = " "){
		for (int i = 0; i < margin; ++i){
			if (i == (margin - 1) / 2){
				std::cout << line_number;
			} else {
				std::cout << " ";
			}
		}
	}


	void print_column_numbers(){
		std::cout << std::string(margin, ' ') << "   1  " << "   2  " << "   3  ";
		std::cout << "\n";
	}





	void myclear(){
		std::cout << std::string(100, '\n');
	}


	void switch_player(){
		current_player = (current_player + 1) % 2;
	}


	void print_final_message(){
		std::cout << nameArr[current_player] << " is the winner!!!" << std::endl;
		std::cout << "Game Over" << std::endl;
	}

	void print_draw_message(){
		std::cout << "How boring.. a Draw." << std::endl;
		mySleep(2);
	}



	bool game_over(){
		int tempx, tempy;
		for (int x = 0; x < 3; ++x){
			for (int y = 0; y < 3; ++y){
				if (board[x][y] != current_player) { continue; }
				
				tempx = x;
				tempy = y;

				for (int i = 0; i < 8; ++i){
					int xdelta = DIR[i][0];
					int ydelta = DIR[i][1];
					
					for (int i = 0; i < 3; ++i){
						if (i == 2){ //Chain of 3 found; current player has won
							return true;
						}
						if (!in_range(tempx + xdelta, tempy + ydelta) || board[tempx + xdelta][tempy + ydelta] != current_player){
							break;
						} else {
							tempx += xdelta;
							tempy += ydelta;
						}
					}
					
				}
			}
		}

		return false;
	}


	
	bool in_range(int x, int y){
		return x >= 0
				&& x < 3 
				&& y >= 0 
				&& y < 3;
	}


	bool board_full(){
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 3; ++j){
				if (board[i][j] == -1){
					return false;
				}
			}
		}
		return true;
	}


	void delete_board(){
		for (int i = 0; i < 3; ++i){
			delete[] board[i];
		}
		delete[] board;
	}


	~Game(){
		delete[] nameArr;
		delete[] symbolArr;
		delete_board();
	}

};






int main(){
	while (true){
		Game game;
		game.start();

		while (true){
			std::string answer;
			std::cout << "Would you like to play again? y/n: ";
			std::cin >> answer;

			if (answer == "y"){
				break;

			} else if (answer == "n"){
				std::cout << "Okay, goodbye!" << std::endl;
				mySleep(1);
				return 0;

			} else {
				std::cout << "Invalid response: try again" << std::endl;
				mySleep(1);
			}
		}

	}
	
	return 0;
}
