// stringstream::str
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <vector>
#include <functional>
#include <chrono>
#include <cmath>
#include <memory>
#include <cstdlib>




#define for_i(size) for(int i = 0; i < size; i++)
#define for_j(size) for(int j = 0; j < size; j++)

using namespace std;


template <typename T, typename T2 = string>
void print(T message, T2 val = " "){
	cout << message << " " << val << endl;
}

template <class T> const T& mymax (const T& a, const T& b) {
	return (a<b)?b:a;     // or: return comp(a,b)?b:a; for version (2)
}

float my_random(float minval = 0,float maxval = 1){
	float r;
	// random float from minval to maxval
	r = minval + static_cast <float> (rand()) /
							( static_cast <float> (RAND_MAX/(maxval-minval)));
	return r;
}

#include "DynamicList.hpp"

// template <class T>
class Vec : public DynamicList<float>{
	protected:
		float length;
	
		void cal_length(){
			float sum = 0;
			for_i(size){
				sum += l_buffer[i]*l_buffer[i];
			}
			this->length = sqrt(sum);
		}
	
	public:
		Vec(const float* content,unsigned int size, string name = "")
			:DynamicList<float>(content, size, name){
			cal_length();
		}

		Vec(unsigned int size, string name = "")
			:DynamicList<float>(size, name){
				cal_length();
		}
	
		void append (float& val_p, int size){
			DynamicList::append(val_p,size);
			cal_length();
		}

		void print_length(){
			cal_length();
			cout << "Length of "<<name<< ": "<<length<<"\n";
		}

};

struct Shape{
	unsigned int rows;
	unsigned int cols;

	void print(){
		cout << "[" << rows << ", " << cols << "]" << endl;
	}
};


template <class T> 
class Array{
	private:
		DynamicList<T>* buffer;

	protected:
		unsigned int ROWS;
		unsigned int COLS;
		Shape shape;
		
		

	public:
		string name;

		
		Array(){

		}

		Array(unsigned int rows,unsigned int cols, string name = ""):name(name){
			ROWS = rows;
			COLS = cols;
			shape.rows = ROWS;
			shape.cols = COLS;
			buffer = new DynamicList<T>[ROWS];
			for_i(ROWS){
				buffer[i].resize(COLS);
				// buffer[i].fill_random();
				string name = "Row"+to_string(i);
				buffer[i].name = name;
			}
			
		}

		//Deconstructor
		~Array(){
			delete[] buffer;
		}
		

		// Copy Contructor 
		Array(Array<T>& other) 
			:ROWS(other.ROWS), COLS(other.COLS), name(other.name){
			shape.rows = ROWS;
			shape.cols = COLS;
			buffer = new DynamicList<T>[ROWS];
			for_i(ROWS){
				string name = "Row"+to_string(i);
				buffer[i].resize(COLS);
				buffer[i].name = name;
				for_j(COLS){
					buffer[i].assign(j,other[i][j]);
				}
			}
		}
		
		void init_random(unsigned int rows,unsigned int cols, float minval = 0, float maxval = 0 ,string name = ""){
			ROWS = rows;
			COLS = cols;
			shape.rows = ROWS;
			shape.cols = COLS;
			this-> name = name;
			buffer = new DynamicList<T>[rows];
			for_i(rows){
				buffer[i].resize(cols);
				buffer[i].fill_random(minval,maxval);
				string name = "Row_"+to_string(i);
				buffer[i].name = name;
			}
			
		}
		void init_zeros(unsigned int rows,unsigned int cols, string name = ""){
			ROWS = rows;
			COLS = cols;
			shape.rows = ROWS;
			shape.cols = COLS;
			this-> name = name;
			buffer = new DynamicList<T>[rows];
			for_i(rows){
				buffer[i].resize(cols);
				buffer[i].fill_zeros();
				string name = "Row_"+to_string(i);
				buffer[i].name = name;
			}
			
		}
		void fill_random(float minval = 0, float maxval = 0){
			for_i(ROWS){
				for_j(COLS){
					assign(i,j, (T) my_random(minval,maxval));
				}
			}
		}
		void fill_zeros(){
			for_i(ROWS){
				for_j(COLS){
					assign(i,j, (T) 0.f);
				}
			}
		}

		void print_shape(){
			cout << "Shape: ";
			this->shape.print();
		}

		Shape get_shape(){
			Shape s;
			s.rows = ROWS;
			s.cols = COLS;
			return s;
		}

		void assign(unsigned int row, unsigned int col, const T& val){
			buffer[row].assign(col, val);
		}



		void set_prec(unsigned int val){
			for_i(ROWS){
				buffer[i].set_prec(val);
			}
		}

		void print(){
			
			unsigned int rows_to_print = ROWS;
			if (rows_to_print>10){
				rows_to_print = 10;
			}

			// spacing 
			string spacing = "   ";

			// printing
			cout << name << ":" << endl;
			for_i(rows_to_print){
				cout << spacing;
				buffer[i].print();
			}
			if (rows_to_print<ROWS){
				cout << spacing <<buffer[rows_to_print].name << " [..]\t: .....\n"; 
			}
			print_shape();
		}

		Array<T>& t(){
			Array<T>* new_array = new Array<T>;
			new_array->init_zeros(COLS,ROWS,name+".T");

			for_i(ROWS){
				for_j(COLS){
					auto val = buffer[i][j];
					new_array->assign(j,i,val);
				}
			}

			return *new_array;
		}


		DynamicList<T>& operator [] (unsigned int row){
			return buffer[row];
		}

		Array<T>& operator * (Array& other){
			if (this->shape.cols!=other.shape.rows){
				throw std::invalid_argument( "\nErrorSP: \n   Shape missmatch. Cols of A ["+to_string(this->shape.cols)+"] != Rows of B ["+to_string(other.shape.rows)+"].");
			}
			else{
				int new_rows = ROWS;
				int new_cols = other.COLS;
				// create new matrix with correct shape
				Array<T>* new_mat = new Array<T>;
				new_mat->init_zeros(new_rows, new_cols, name+"*"+other.name);

				// fill matrix
				T value;
				// iter all elements of the new matrix
				for_i(new_rows){
					for_j(new_cols){
						value = 0;
						
						// for each of these iter through slices of both old matrices
						for(int vec_index = 0; vec_index < other.ROWS; vec_index++){
							//vec index goes through cols of first matrix and cols of other
							value += buffer[i][vec_index]*other[vec_index][j];
						}
						
						
						new_mat->assign(i,j,value);
					}
					
				}

				return *new_mat;
			}

			
		}


		Array operator * (float value){
			Array<T> new_array(ROWS,COLS,name+"*"+to_string(value));
			for_i(ROWS){
					for_j(COLS){
						new_array.assign(i,j,this->buffer[i][j]*value);
					}
			}
			return new_array;
		}

		Array operator + (Array& other){
			Array<T> new_array(ROWS,COLS,name+"+"+other.name);
			if (other.COLS != this-> COLS || other.ROWS != this->ROWS){
				throw std::invalid_argument( "\nErrorAdd: \n   Shape missmatch. Cols and Rows of A != Cols and Rows of B.");
			}
			
			for_i(ROWS){
					for_j(COLS){
						new_array.assign(i,j,this->buffer[i][j]+other.buffer[i][j]);
					}
			}
			return new_array;
		}
		
		Array operator - (Array& other){
			Array<T> new_array(ROWS,COLS,name+"+"+other.name);
			if (other.COLS != this-> COLS || other.ROWS != this->ROWS){
				throw std::invalid_argument( "\nErrorAdd: \n   Shape missmatch. Cols and Rows of A != Cols and Rows of B.");
			}
			
			for_i(ROWS){
					for_j(COLS){
						new_array.assign(i,j,this->buffer[i][j]-other.buffer[i][j]);
					}
			}
			return new_array;
		}
		
		void operator = (Array other){
			for_i(ROWS){
				for_j(COLS){
					this->buffer[i][j] = other[i][j];
				}
			}
		}
};
