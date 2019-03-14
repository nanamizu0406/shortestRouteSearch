#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<utility>
#include<random>

using point=std::pair<unsigned, unsigned>;
class Dijkstra;

class Field{
private:
	enum state{
		EMPTY=0,
		POINT=1,
		START=2,
		GOAL=3
	};
	std::vector<std::vector<int>> field;
	std::vector<point> list;
	const unsigned wight=20;
	const unsigned height=30;
	const unsigned nodesNum=7;
	point start;
	point goal;
private:
	point random() const;
public:
	friend Dijkstra;
public:
	Field();
	~Field();
	void inits();
	void print() const;
}field;

class Node{
private:
public:
	Node();
	~Node();
	std::vector<int> edgesTo;
	std::vector<int> edgesCost;
	bool isComfirmNode;
	int cost;
};

class Dijkstra{
private:
	std::vector<Node> nodes;
public:
	Dijkstra();
	~Dijkstra();
	void inits();
	unsigned distance(const point& p1, const point& p2) const;
	unsigned search();
}dijkstra;

int main(int argc, char* argv[]){
	field.print();
	return 0;
}

Field::Field(){
	this->inits();
}
Field::~Field(){}
point Field::random() const{
	std::random_device rnd;
	std::mt19937_64 mt(rnd());
	std::uniform_int_distribution<> rand_y(0,this->height-1);
	std::uniform_int_distribution<> rand_x(0,this->wight-1);
	const unsigned x=rand_x(mt);
	const unsigned y=rand_y(mt);
	point random=std::make_pair(y, x);
	return random;
}
void Field::inits(){
	this->field.resize(this->height);
	std::for_each(this->field.begin(), this->field.end(), [this](auto& vec){
			vec.resize(this->wight);
		});
	std::for_each(this->field.begin(), this->field.end(), [this](auto& vec){
			std::fill(vec.begin(), vec.end(), EMPTY);
		});
	
	this->start=std::make_pair(0, 0);
	this->goal=std::make_pair(this->height-1, this->wight-1);
	
	std::cout<<"S("<<this->start.second+1<<","<<this->start.first+1<<") ";
	std::cout<<"G("<<this->goal.second+1<<","<<this->goal.first+1<<") ";
	this->list.push_back(this->start);
	
	point obj;
	for(int k=0;k<this->nodesNum;k++){
		obj=this->random();
		this->list.push_back(obj);
		if(obj==this->start||obj==this->goal)
			continue;
		std::cout<<"("<<obj.first+1<<","<<obj.second+1<<") ";
		this->field.at(obj.first).at(obj.second)=POINT;
	}
	std::cout<<std::endl;
	
	this->list.push_back(this->goal);
	this->field.at(this->start.first).at(this->start.second)=START;
	this->field.at(this->goal.first).at(this->goal.second)=GOAL;
}
void Field::print() const{
	for(int i=0;i<this->field.size();i++){
		for(int j=0;j<this->field.at(0).size();j++){
			switch(unsigned state=this->field[i][j]){
			case EMPTY:
				std::cout<<"　";
				break;
			case POINT:
				std::cout<<"＠";				
				break;
			case START:
				std::cout<<"Ｓ";				
				break;
			case GOAL:
				std::cout<<"Ｇ";				
				break;
			default:
				break;
			}
		}
		std::cout<<std::endl;
	}
}

Node::Node():isComfirmNode(false),cost(-1){}
Node::~Node(){}

Dijkstra::Dijkstra(){}
Dijkstra::~Dijkstra(){}
void Dijkstra::inits(){
	
}

unsigned Dijkstra::search(){
	
}
