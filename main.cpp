#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>
#include<chrono>
#include<iomanip>
#include<vector>
#include<ctime>
#include<cmath>


using namespace std;

typedef struct Taxi{ //it stores Taxi data
	float longitude;
	float latitude;
	float distance;
}Taxi;

int left(int i) //gives left node index
{
	return (2*i+1);
}

int right(int i) //gives right node index
{
	return (2*i+2);
}

int parent(int i) //gives parent index
{
	return((i-1)/2);
}

void heapify(vector<Taxi*> &taxiHeap, unsigned int index) //heapifies the subtree
{ 
    unsigned int left_node = left(index); //keeps index of left child
    unsigned int right_node = right(index);  //keeps index of right child
    unsigned int smallest = index; //keeps smallest node's index
    if (left_node < taxiHeap.size() && taxiHeap[left_node]->distance < taxiHeap[index]->distance) //if left child's distance is smaller than smallest set it smallest
        smallest = left_node; 
    if (right_node < taxiHeap.size() && taxiHeap[right_node]->distance < taxiHeap[smallest]->distance) //if right child's distance is smaller than smallest set it smallest
        smallest = right_node; 
    if (smallest != index) //if the smallest element is not the root of the subtree
    { 
        swap(taxiHeap[index], taxiHeap[smallest]);//swap the smallest element of subtree to the subtree's root
        heapify(taxiHeap, smallest); //heapify the swapped child's subtree
    } 
}

void insertTaxi(vector<Taxi*> &heap, Taxi* new_taxi) //insert a new node to the heap
{ 
	heap.push_back(new_taxi); //add to end of the heap
	int i = heap.size()-1; //take the index of the new added element
    while (i > 0 && heap[parent(i)]->distance > heap[i]->distance) //place it to the correct area 
    { 
       swap(heap[i], heap[parent(i)]); 
       i = parent(i); 
    } 
}

Taxi* extractMin(vector<Taxi*> &heap) //extracts the root from the heap
{ 
	Taxi* deleted_taxi = NULL;
    if (heap.size() > 0) //if there is only root
    {	
		deleted_taxi = heap[0]; //take its address
		heap[0] = heap[heap.size()-1]; //take last element to the root 
		heap.pop_back(); //delete the old last element
   		heapify(heap,0); //heapify the tree for the new root
    } 
    return deleted_taxi; //return deleted root
}

void decreaseDistance(vector<Taxi*> &heap, unsigned int index) //decrease the distance 0.01 for given index
{ 
	if(heap.size() > 0 &&(index >= 0 && index < heap.size())) //if index is valid and heap is not empty
	{
		heap[index]->distance -= 0.01; //decrease distance
		int i = index;
   		while (i > 0 && heap[parent(i)]->distance > heap[i]->distance) //replace the node according to the its new distance
    	{ 
       		swap(heap[i], heap[parent(i)]); 
       		i = parent(i); 
   		} 
	}
    
}

void delete_heap(vector<Taxi*> &heap)
{
	for(unsigned int i = 0; i < heap.size(); i++)
	{
		delete heap[i];
	}
}

int main(int argc, char* argv[])
{
	vector<Taxi*> taxiHeap; //heap for keep taxis
	srand(time(NULL));
	stringstream operation(argv[1]);
	unsigned int N = 0;
	operation >> N; //take the N value
	stringstream prob(argv[2]);
	float p = 0.0;
	prob >> p; //take the p value

	unsigned int number_of_addition = 0;
	unsigned int number_of_updates = 0;

    ifstream file;
	file.open("locations.txt");
    string line;
    if (!file){
		cerr << "File cannot be opened!";
		exit(1);
	}
	getline(file, line); //this is the header line

	auto start = chrono::high_resolution_clock::now();
	ios_base::sync_with_stdio(false);

	for(unsigned int i = 0; i<N;) //operations
	{
		float randomnumber = (float)(rand() % 101) / (float)100; //generate a float number between 0 and 1
		if(randomnumber >= 0 && randomnumber <= p && taxiHeap.size() != 0) //if heap is not empty and updating prob occurs
		{
			int randomTaxi = rand() % taxiHeap.size(); //select a random taxi
			decreaseDistance(taxiHeap, randomTaxi); //decrease its distance
			number_of_updates++;
			i++;
		}
		else if(randomnumber > p && randomnumber <= 1) //if p does not occur add a new taxi
		{
			Taxi* new_taxi = new Taxi; //dynamic taxi node
			file >> line; //longitude 
			stringstream dummy1(line);
			float longitude = 0;
			dummy1 >> longitude; //converts string to float
			new_taxi->longitude = longitude;
			file >> line; //latitude (float)
			stringstream dummy2(line);
			float latitude = 0;
			dummy2 >> latitude; //converts string to float
			new_taxi->latitude = latitude;
			new_taxi->distance = sqrt(longitude*longitude + latitude*latitude);
			getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
			insertTaxi(taxiHeap, new_taxi); //insters new taxi
			number_of_addition++;
			i++;
		}
		if(i != 0 && i%99 == 0) //call taxi for each 100 operations
		{
			Taxi* calledTaxi = extractMin(taxiHeap); //call taxi
			cout << "Distance of the called taxi: " << calledTaxi->distance << endl; //print its info
			delete calledTaxi; //delete dynamic node
		}	
	}

	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
	time_taken *= 1e-6;
	cout << "The number of taxi additions: " << number_of_addition << endl;
	cout << "The number of updates: " << number_of_updates << endl;
	cout << fixed << time_taken << setprecision(9) << " millisec" << endl;
	delete_heap(taxiHeap); //delete whole heap
	file.close();


    return 0;
}