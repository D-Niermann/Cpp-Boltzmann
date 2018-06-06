#include "include/mnist_reader.hpp"
#include "include/Array.hpp"
#include <chrono>
#include <typeinfo>
#include <cmath>


using namespace std;

struct Timer{
	private:
		// define start and end time
		std::chrono::time_point<std::chrono::steady_clock> start,end;
		// define duration object
		std::chrono::duration<float> duration;
		// name of the timer
	public:
		string name;
		int a = 3;
		float b = (float)a;
	Timer(){
		start = std::chrono::high_resolution_clock::now();
		
	}
	Timer(string name) {
		this -> name = name;
		start = std::chrono::high_resolution_clock::now();
		::print(name);
	}
	~Timer(){
		end 	 = std::chrono::high_resolution_clock::now();
		duration = end - start;
		cout << "Done: " << name << "(" <<duration.count()<< "s)" << endl;
	}
	
};
template <typename T>
void load_data(Array<T> & arr1, Array<T> & arr2){
	Timer t1("Loading and Assigning train_data array...");
	// load mnist dataset
	mnist::MNIST_dataset<std::vector, std::vector<float>, uint8_t> dataset = mnist::read_dataset<std::vector, std::vector, float, uint8_t>("/Users/Niermann/Documents/C++/Cpp-Boltzmann/MNIST_LOADER");

    // std::cout << "Nbr of training images = " << dataset.training_images.size() << std::endl;
    // std::cout << "Nbr of training labels = " << dataset.training_labels.size() << std::endl;
    // std::cout << "Nbr of test images = " << dataset.test_images.size() << std::endl;
    // std::cout << "Nbr of test labels = " << dataset.test_labels.size() << std::endl;
	
    for (int image = 0; image < 60000; ++image){
		int m = 0;
		for_i(784){
			arr1.assign(image,m,dataset.training_images[image][i]/255);
			if (image < 10000){
				arr2.assign(image,m,dataset.test_images[image][i]/255);
			}
			m+=1;
		}
	}

}

template <typename T>
Array<T>& sigmoid(Array<T>& array,const float & temp){
	Shape shape = array.get_shape();
	Array<T>* new_array = new Array<T>;
	new_array->init_zeros(shape.rows,shape.cols,"sigmoid("+array.name+")");
	for_i(shape.rows){
		for_j(shape.cols){
			// cout <<shape.rows<<"," <<i <<endl;
			auto x = array[i][j];
			new_array->	assign(i,j,1/(1+exp(-x/temp)));
		}
	}
	return *new_array;
}

int main(){
	srand(time(0));
	

	const float TEMP = 0.1;
	const float N_FR = 2;
	// load data
	Array<float> train_data(60000,784,"train_data");
	Array<float> test_data(10000,784,"train_data");	
	load_data(train_data, test_data);




	//Create 	
	Array<float>* Layer = new Array<float>[2];
	Array<float>* Layer_save = new Array<float>[2];
	int n_layer = 2;

	int v_units = 784;
	int h_units = 20;

	// Layer
	Layer[0].init_random(1,v_units,-0.001,0.001,"v_layer");
	Layer[1].init_random(1,h_units,-0.001,0.001,"h_layer");
	// Weights
	Array<float> W(v_units,h_units,"Weights");
	Array<float> W_T(h_units,v_units,"Weights.T");
	W.fill_random(-0.001,0.001);

	// Gradients
	Array<float> pos_grad(v_units,h_units,"pos_grad");
	Array<float> neg_grad(v_units,h_units,"neg_grad");

	Array<float> h_input(1,h_units);
	Array<float> v_input(1,v_units);

	
	print("Starting Epoch");
	for(int image = 0; image < 1; ++image){
		Layer[0][0] = train_data[image];
		// update layer 1
		h_input = sigmoid(Layer[0]*W,TEMP);
		Layer[1] = h_input;
		// update Layer 0
		W_T = W.t();
		v_input = sigmoid(Layer[1]*W_T,TEMP);
		Layer[0] = v_input;

		// save layers
		for_i(2){
			Layer_save[i] = Layer[i];
		}

		// uodate pos grad
		// pos_grad = Layer_save[0].t()*Layer_save[1];
		pos_grad.print();

		// freeruning steps
		for (int step = 0; step < N_FR; ++step){
			h_input = sigmoid(Layer[0]*W,TEMP);
			Layer[1] = h_input;
			v_input = sigmoid(Layer[1]*W_T,TEMP);
			Layer[0] = v_input;
		}



	}

	print("Finished Epoch");


	/*
	Array<float> a(1,2,"A");
	Array<float> b(1,2,"B");
	a.fill_random(0,10);
	b.fill_random(0,2);
	a = b;





	DynamicList<float> list1(10,"A");
	list1.fill_random(0,10);
	print("Copying list:");
	DynamicList<float> list2(10,"B");

	
	
	list2 = list1;

	list1.print();
	list2.print();

	list1.fill_random(0,1);

	*/

	// -------------------------------------------------
	// float result = vector1 * vector2	;
	// print("Result: ",result);



	// Array<float>* Weights = new Array<float>[3];
	// Weights[1].init_random(1,6,0,1,"W1");
	// Weights[1].print();
	// Weights[0].init_random(6,1,-1,1,"W2");
	// Weights[0].print();

	// auto new_mat = Weights[1]*Weights[0];
	// new_mat.print();

	// Weights[0]
	
	// auto dyn_p = new DynamicList<float>[10];
	// for_i(10){
	// 	dyn_p[i].name = "Vec"+to_string(i);
	// }
	// dyn_p[2].append(2.f,1);

	// dyn_p[2].print();


	// float res = SP(vector1,vector2);
	// // print("Res",res);
	// cout << "---------" << endl;
	// vector1.print();
	// vector1.print_length();
	return 0;
}