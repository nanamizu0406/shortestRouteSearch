#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<random>
#include<chrono>
#include<cmath>
#include<GL/glut.h>

const unsigned cellSize=20;

void inits();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display();

using point=std::pair<int, int>;

point operator+(const point &lhs, const point &rhs){
	point p = { lhs.first + rhs.first, lhs.second + rhs.second };
	return p;
}

void sort(std::vector<point>& vec);

class Timer{
private:
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
public:
	void begin();
	void stop();
	void disp() const;
}timer;

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
	void printPointCoord() const;
	void glutField() const;
	void glutPoint() const;
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
	unsigned id;
	unsigned from;
	bool isComfirmNode;
	double cost;
};

class Search{
private:
	std::vector<Node> nodes;
	static const unsigned branch=3;
public:
	Search();
	~Search();
	void inits();
	double distance(const point& p1, const point& p2) const;
	void dijkstra();
	void glutBranch() const;
	void printNode() const;
	void printCost() const;
}search;

int main(int argc, char* argv[]){	
	field.inits();
	search.inits();

	//	search.printCost();
	//	field.printPointCoord();
	
	timer.begin();
	search.dijkstra();
	timer.stop();
	glutInit(&argc, argv);
	glutCreateWindow("search_route");
	inits();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

void inits(){
	static const unsigned WindowSizeWight=800;
	static const unsigned WindowSizeHeight=800;

	static const unsigned WindowPossitionWight=100;
	static const unsigned WindowPossitionHeight=100;

	gluOrtho2D(0, 100, 100, 0);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowPosition(WindowPossitionWight, WindowPossitionHeight);
	glutInitWindowSize(WindowSizeWight, WindowSizeHeight);

	glClearColor(0.0, 0.0, 0.0, 1.0);
}
void resize(int w, int h){
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w-0.5, (GLdouble)h-0.1, -0.5, -1.0, 1.0);
}
void keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q':
	case 'Q':
	case '\033':
		std::exit(0);
		break;
	case 'r':
	case 'R':
			field.inits();
			search.inits();
			timer.begin();
			search.dijkstra();
			timer.stop();
			glutPostRedisplay();
		break;
	default:
		break;
	}
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	field.print();
	search.printNode();
	timer.disp();
	
	field.glutField();
	field.glutPoint();
	search.glutBranch();
	
	glutSwapBuffers();
	glFlush();
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

void Timer::begin(){
	this->start=std::chrono::system_clock::now();
}
void Timer::stop(){
	this->end=std::chrono::system_clock::now();
}
void Timer::disp() const{
	unsigned val=std::chrono::duration_cast<std::chrono::microseconds>(this->end-this->start).count();
	std::cout<<val<<"micro"<<std::endl;
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
	this->list.clear();
	this->field.clear();
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
		bool result=(itr1==this->field.at(obj.second).end()&&itr2==this->field.at(obj.second).end()&&itr3==this->field.at(obj.second).end());
		if(!result)
			continue;
		this->field.at(obj.second).at(obj.first)=POINT;
		this->list.push_back(obj);
		k++;
	}
	sort(this->list);
}
void Field::print() const{
	for(int i=0;i<this->field.size();i++){
		for(int j=0;j<this->field.at(0).size();j++){
			switch(unsigned state=this->field[i][j]){
			case EMPTY:
				std::cout<<"　";
				break;
			case POINT:
				std::cout<<"\x1b[31m";
				std::cout<<"＠";
				std::cout<<"\x1b[39m";
				break;
			case START:
				std::cout<<"\x1b[34m";
				std::cout<<"Ｓ";
				std::cout<<"\x1b[39m";
				break;
			case GOAL:
				std::cout<<"\x1b[34m";
				std::cout<<"Ｇ";
				std::cout<<"\x1b[39m";				
				break;
			default:
				break;
			}
		}
		std::cout<<std::endl;
	}
}
void Field::printPointCoord() const{
	std::for_each(this->list.begin(), this->list.end(), [this](auto obj){;
			std::cout<<"("<<obj.first+1<<","<<obj.second+1<<") ";
		});
	std::cout<<std::endl;
}
void Field::glutField() const{
	static const unsigned lineSize=1;
	
	glColor3f(0.0f, 1.0f, 0.0);
	glLineWidth(lineSize);
	glBegin(GL_LINES);
	for(int i=0;i<=this->wight;i++){
		glVertex2i(cellSize*i, 0);
		glVertex2i(cellSize*i, cellSize*this->wight);
	}
	for(int j=0;j<=this->height;j++){
		glVertex2i(0, cellSize*j);
		glVertex2i(cellSize*this->height, cellSize*j);
	}
	glEnd();
}
void Field::glutPoint() const{
	static const double val=cellSize/2;
	static const unsigned pointSize=19;
	
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for(int i=0;i<this->field.size();i++){
		for(int j=0;j<this->field.at(0).size();j++){
			switch(this->field.at(i).at(j)){
			case START:
			case GOAL:
				glColor3f(0.0f, 0.0f, 8.0f);
				glVertex2d(val+cellSize*j, val+cellSize*i);
				break;
			case POINT:
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex2d(val+cellSize*j, val+cellSize*i);				
				break;
			default:
				break;
			}
		}
	}
	glEnd();
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
	
	unsigned y1=p1.second;
	unsigned y2=p2.second;

	//---ポイントと経路がかぶらないようにするための例外処理---
	//---最悪やらなくても良い---
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
	this->nodes.clear();
	this->nodes.resize(field.sizeList());
	for(int i=0;i<this->nodes.size();i++){
		nodes.at(i).id=i;
	}
	for(int k=0;k<this->nodes.size()-1;k++){
		int l=1;
		while(l<=branch){
			if(k+l>=field.sizeList())
				break;
			/*
     	---ポイントと経路がかぶらないようにするための例外処理---
			*/
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
}
double Search::distance(const point &p1, const point &p2) const{
	return std::sqrt((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second));
}
void Search::dijkstra(){
	while(true){
		int process=-1;
		for(int i=0;i<this->nodes.size();i++){
			if(this->nodes.at(i).isComfirmNode||this->nodes.at(i).cost<0)
				continue;
			if(process==-1){
				process=this->nodes.at(i).id;
				continue;
			}
			if(this->nodes.at(i).cost<this->nodes.at(process).cost){
				process=this->nodes.at(i).id;
			}
		}	
		if(process==-1)
			break;
		this->nodes.at(process).isComfirmNode=true;
		for(int i=0;i<this->nodes.at(process).edgesTo.size();i++){
			unsigned id=this->nodes.at(process).edgesTo.at(i);
			double cost=this->nodes.at(process).cost+this->nodes.at(process).edgesCost.at(i);
			if(this->nodes.at(id).cost<0||this->nodes.at(process).edgesCost.at(i)>cost){
				this->nodes.at(id).cost=cost;
				this->nodes.at(id).from=this->nodes.at(process).id;
			}
		}
	}
}
void Search::glutBranch() const{
	std::vector<unsigned> log;
	log.push_back(this->nodes.at(this->nodes.size()-1).id);
	unsigned from=this->nodes.at(this->nodes.size()-1).from;
	while(true){
		log.push_back(from);		
		if(from==0)
			break;
		from=this->nodes.at(from).from;
	}
	std::reverse(log.begin(), log.end());

	static const double val=cellSize/2;
	static const unsigned lineSize=1;
	static const unsigned pointSize=10;
	
	glLineWidth(lineSize);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	for(int i=0;i<log.size()-1;i++){
		unsigned val1=log.at(i);
		point obj1=field.getList(val1);
		unsigned val2=log.at(i+1);
		point obj2=field.getList(val2);
		glVertex2d(val+obj1.first*cellSize, val+obj1.second*cellSize);
		glVertex2d(val+obj2.first*cellSize, val+obj2.second*cellSize);
	}
	glEnd();
	
	glPointSize(pointSize);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<log.size()-1;i++){
		unsigned val3=log.at(i);
		point obj3=field.getList(val3);
		glVertex2d(val+obj3.first*cellSize, val+obj3.second*cellSize);
	}
	glEnd();
}
void Search::printNode() const{
	std::vector<unsigned> log;
	log.push_back(this->nodes.at(this->nodes.size()-1).id);
	unsigned from=this->nodes.at(this->nodes.size()-1).from;
	while(true){
		log.push_back(from);		
		if(from==0)
			break;
		from=this->nodes.at(from).from;
	}
	std::reverse(log.begin(), log.end());
	std::for_each(log.begin(), log.end(), [&](auto val){
			if(val==0||val==this->nodes.size()-1){
				std::cout<<"\x1b[34m";
				if(val==0){
					std::cout<<"S";
				}
				if(val==this->nodes.size()-1){
					std::cout<<"G";
				}			
				std::cout<<"\x1b[39m";
			}
			else
				std::cout<<val;
			point obj=field.getList(val);
			std::cout<<"("<<obj.first+1<<","<<obj.second+1<<") >> ";
		});
	std::cout<<std::fixed;
	std::cout<<this->nodes.at(this->nodes.size()-1).cost<<"cost"<<std::endl;
}
void Search::printCost() const{
	unsigned num=0;
	std::cout<<"-------------------------------"<<std::endl;
	std::for_each(this->nodes.begin(), this->nodes.end(), [&,this](auto& node){
			std::cout<<num++<<" |";
			std::for_each(node.edgesTo.begin(), node.edgesTo.end(), [this](auto& val){
					std::cout<<val<<" ";
					});
			for(int k=0;k<field.sizeList()-2-node.edgesTo.size();k++)
				std::cout<<"  ";
			std::cout<<"| ";
			std::for_each(node.edgesCost.begin(), node.edgesCost.end(), [&,this](auto& score){
					std::cout<<std::fixed;
					std::cout<<"("<<score<<") ";
				});
			std::cout<<std::endl;
		});
	std::cout<<"-------------------------------"<<std::endl;
}


