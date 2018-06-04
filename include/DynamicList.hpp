template <class T>
class DynamicList{
	protected:
		T* l_buffer;
		unsigned int size;
		int PRINT_PRECISION = 5;
	public:
		string name;


	public:
		// Constructors
		DynamicList(const T* content,unsigned int size, string name = "")
			:name(name), size(size)
		{
			l_buffer = new T[size];
			// this->size = size;
			//this -> name = name;
			for (int i = 0; i<size; i++){
				l_buffer[i] = content[i];
			}
		}

		
		DynamicList(){
			l_buffer = NULL;
			size = 0;
		}


		DynamicList(unsigned int size, string name ="") : name(name), size(size){
			l_buffer = new T[size];

		}


		// De-Constructor
		virtual ~DynamicList(){
			delete[] l_buffer;
		}

		//Copy Constructor
		DynamicList(DynamicList<T>& other)
			:size(other.size), name(other.name+"_Copy"){
			//memcpy from other to self
			// size = other.size;
			// name = other.name + "_Copy";
			l_buffer =  new T[size];

			for(int i = 0; i<size;i++){
				l_buffer[i] = other.l_buffer[i];
			}
		}

		
		virtual void print(){
			cout << name << " [" << size << "]" << " :[";
			
			if (std::is_same<T, char>::value){
				for (int i = 0; i<size-1; i++){
					cout << *(l_buffer+i);
				}
			}
			else{
				for (int i = 0; i<size; i++){
					if (i>3 && i<(size-3)){
						cout << "..., ";
						i=size-3;
					}
					T number = *(l_buffer+i);
					string num_str = to_string(number);
					int str_len = std::strlen(&num_str[0]);
					auto maxi = mymax(0,(PRINT_PRECISION+1-str_len));
					if (std::is_same<T, float>::value){
						for_i(maxi){
							num_str += '0';
						}
					}
					cout << num_str.substr(0,PRINT_PRECISION+1);
					if (i<size-1){
						cout <<", ";
					}
				}
			}

			cout << "]\n";
			// cout << *(l_buffer+size-1) << "] "<<"\n";
		}


		virtual void append(const T& value_p, unsigned int size){
			
			unsigned int my_size = this->size;
			T* l_buffer2 = new T[my_size+size];
			
			
			// memcpy(l_buffer2,l_buffer,my_size);
			for (int i = 0; i<my_size;i++){
				l_buffer2[i] = l_buffer[i];
			}
			
			for (int i = my_size; i<(my_size + size); i++){
				// cout<<name << " Writing: "<<i-my_size<<" "<<value_p[i-my_size]<<endl;
				l_buffer2[i] = value_p+i-my_size;
			}
			// delete[] l_buffer;

			this -> size += size;
			delete[] l_buffer;
			this -> l_buffer = l_buffer2;
		}


		void resize(unsigned int new_size){
			unsigned int my_size = this->size;
			T* l_buffer2 = new T[new_size];
			
			unsigned int iter_size = min(my_size,new_size);
			
			// memcpy(l_buffer2,l_buffer,my_size);
			for (int i = 0; i<iter_size;i++){
				l_buffer2[i] = l_buffer[i];
			}
			
			// delete[] l_buffer;

			this -> size = new_size;
			delete[] l_buffer;
			this -> l_buffer = l_buffer2;
		}

		virtual void set_prec(unsigned int prec){
			this->PRINT_PRECISION = prec;
		}

		void fill_random(float minval=0, float maxval=1){
			//fill
			for_i(size){
				l_buffer[i] = (T) my_random(minval,maxval);
			}
		}
		void fill_zeros(){
			//fill
			for_i(size){
				l_buffer[i] = (T) 0.f;
			}
		}


		void assign(unsigned int pos, const T& value){
			l_buffer[pos] = value;
		}

		int get_size(){
			return (int) size;
		}

		// Operator Overloads
		T& operator [] (unsigned int pos){
			return (l_buffer[pos]);
		}


		T operator * (DynamicList<T>& other){
			T res = 0;
			int a_size = get_size();
			int b_size = other.get_size();
			if (a_size!=b_size){
				throw std::invalid_argument( "\nError: Vectors must be same length" );
			}
			else{
				for_i(a_size){
					res += l_buffer[i]*other[i];
				}
			}

			return res;
		}
		
		

		
		template <typename T2 = float>
		DynamicList<T> operator * (T2 value){
			DynamicList<T> new_list(size, name);
			
			for_i(size){
				new_list.assign(i, l_buffer[i]*value);
			}

			return new_list;

		}


		template <typename T2 = float>
		DynamicList<T> operator *= (T2 value){
			for_i(size){
				l_buffer[i]*=value;
			}
		}


		template <typename T2 = float>
		DynamicList<T> operator += (T2 value){
			for_i(size){
				l_buffer[i]+=value;
			}
		}

		void operator = (DynamicList<T>& other){

			if (size!=other.size){
				throw std::invalid_argument( "\nError: Vectors must be same length" ); 
			}
			else{
				for_i(size){
					this->l_buffer[i] = other.l_buffer[i];
				}
			}
		}
};