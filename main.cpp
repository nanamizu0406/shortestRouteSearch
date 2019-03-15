#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<random>
#include<chrono>
#include<cmath>

using point=std::pair<int, int>;

point operator+(const point &lhs, const point &rhs){
	point p = { lhs.first + rhs.first, lhs.second + rhs.second };
	return p;
}

void sort(std::vector<point>& vec);

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
	const unsigned wight=10;
	const unsigned height=10;
	const unsigned nodesNum=7;
	point start;
	point goal;
private:
	point random() const;
public:
	Field();
	~Field();
	void inits();
	void print() const;
	bool isPointOnSegment(int x, int y, int x1, int y1, int x2, int y2) const;
	bool searchBetween(const point& p1, const point& p2) const;
	point getList(const unsigned val) const;
	unsigned sizeList() const;
}field;

class Node{
private:
public:
	Node();
	~Node();
	std::vector<int> edgesTo;
	std::vector<double> edgesCost;
	unsigned from;
	bool isComfirmNode;
	double cost;
};

class Search{
private:
	std::vector<Node> nodes;
public:
	Search();
	~Search();
	void inits();
	double distance(const point& p1, const point& p2) const;
	unsigned dijkstra();
}search;

int main(int argc, char* argv[]){
	field.print();
	return 0;
}

void sort(std::vector<point>& vec){
	point temp;
	for(int i=0;i<vec.size()-1;i++){
		for(int j=vec.size()-1;j>i;j--){
			if(vec.at(j-1).second>vec.at(j).second){
				temp=vec.at(j-1);
				vec.at(j-1)=vec.at(j);
				vec.at(j)=temp;
			}
		}
	}
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
	
	this->field.at(this->start.second).at(this->start.first)=START;
	this->field.at(this->goal.second).at(this->goal.first)=GOAL;
	
	this->list.push_back(this->start);
	this->list.push_back(this->goal);
	
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
		this->field.at(obj.second).at(obj.first)=POINT;
		this->list.push_back(obj);
		k++;
	}

	sort(this->list);
	std::for_each(this->list.begin(), this->list.end(), [this](auto obj){
			std::cout<<"("<<obj.first+1<<","<<obj.second+1<<") ";
		});
	std::cout<<std::endl;
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
bool Field::isPointOnSegment(int x, int y, int x1, int y1, int x2, int y2) const{
	  unsigned d;
    if(x1>x2) {
        d=x1,x1=x2,x2=d;
        d=y1,y1=y2,y2=d;
    }
    bool result=(x1<=x&&x<=x2&&((y1<=y2&&y1<=y&&y<=y2)||(y1>y2&&y2<=y&&y<=y1))&&(y-y1)*(x2-x1)==(y2-y1)*(x-x1));
		return result;
}
bool Field::searchBetween(const point &p1, const point &p2) const{
	return false;
	

}
point Field::getList(const unsigned val) const{
	return this->list.at(val);
}
unsigned Field::sizeList() const{
	return this->list.size();
}

Node::Node():isComfirmNode(false),cost(-1),from(0){}
Node::~Node(){}

Search::Search(){
	this->inits();
}
Search::~Search(){}
void Search::inits(){
	this->nodes.resize(field.sizeList());
	for(int k=0;k<this->nodes.size()-1;k++){
		int l=1;
		unsigned num=3;
		while(l<=num){
			if(k+l>=field.sizeList())
				break;
			
			if(field.searchBetween(field.getList(k), field.getList(k+l))){
				num++;
				continue;
			}
			
			this->nodes.at(k).edgesTo.push_back(k+l);
			this->nodes.at(k+l).edgesTo.push_back(k);
			l++;
		}
	}

	double score=0;
	for(int i=0;i<this->nodes.size();i++){
		for(int j=0;j<this->nodes.at(i).edgesTo.size();j++){
			score=this->distance(field.getList(i), field.getList(nodes.at(i).edgesTo.at(j)));
			this->nodes.at(i).edgesCost.push_back(score);
		}
	}
	this->nodes.at(0).cost=0;
	
	unsigned num1=1;
	unsigned num2=0;
	std::cout<<"------------------------"<<std::endl;
	std::for_each(this->nodes.begin(), this->nodes.end(), [&,this](auto& node){
			std::cout<<num1++<<"|";
			std::for_each(node.edgesTo.begin(), node.edgesTo.end(), [this](auto& val){
					std::cout<<val+1<<" ";
					});
			num2=7-node.edgesTo.size();
			for(int k=0;k<num2;k++)
				std::cout<<"  ";
			std::cout<<"| ";
			std::for_each(node.edgesCost.begin(), node.edgesCost.end(), [&,this](auto& score){
					std::cout<<std::fixed;
					std::cout<<"("<<score<<") ";
				});
			std::cout<<std::endl;
		});
	std::cout<<"------------------------"<<std::endl;
}
double Search::distance(const point &p1, const point &p2) const{
	return std::sqrt((p1.first-p2.first)*(p1.first-p2.first)
													 +(p1.second-p2.second)*(p1.second-p2.second));
}
		
unsigned Search::dijkstra(){
	
}
