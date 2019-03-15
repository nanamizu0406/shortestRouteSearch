#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<utility>
#include<random>
#include<chrono>
#include<cmath>

using point=std::pair<unsigned, unsigned>;
class Dijkstra;

class Timer{
private:
public:
};

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
	double distance(const point& p1, const point& p2) const;
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
	std::uniform_int_distribution<> rand_x(0,this->wight-1);
	std::uniform_int_distribution<> rand_y(0,this->height-1);
	const unsigned x=rand_x(mt);
	const unsigned y=rand_y(mt);
	point random=std::make_pair(x, y);
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
	this->goal=std::make_pair(this->wight-1, this->height-1);
	
	std::cout<<"S("<<this->start.first+1<<","<<this->start.second+1<<") ";
	std::cout<<"G("<<this->goal.first+1<<","<<this->goal.second+1<<") ";
	this->field.at(this->start.second).at(this->start.first)=START;
	this->field.at(this->goal.second).at(this->goal.first)=GOAL;
	
	this->list.push_back(this->start);
	
	for(int k=0;k<this->nodesNum;){
		point obj=this->random();
		if(obj==this->start||obj==this->goal)
			continue;
		const auto itr1=std::find(this->field.at(obj.second).begin(), this->field.at(obj.second).end(), POINT);
		const auto itr2=std::find(this->field.at(obj.second).begin(), this->field.at(obj.second).end(), START);
		const auto itr3=std::find(this->field.at(obj.second).begin(), this->field.at(obj.second).end(), GOAL);
		bool result=(itr1==this->field.at(obj.second).end()&&itr2==this->field.at(obj.second).end()
								 &&itr3==this->field.at(obj.second).end());
		if(!result)
			continue;
		std::cout<<"("<<obj.first+1<<","<<obj.second+1<<") ";
		this->field.at(obj.second).at(obj.first)=POINT;
		this->list.push_back(obj);
		k++;
	}
	std::cout<<std::endl;
	this->list.push_back(this->goal);
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
double Dijkstra::distance(const point &p1, const point &p2) const{
	return std::abs(std::pow((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second), 0.5));
}
		
unsigned Dijkstra::search(){
	
}
