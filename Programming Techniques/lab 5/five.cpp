/*! \mainpage <CENTER> Лабораторная работа N5 Выполнил Федоров Сергей Скб172 </CENTER>
 *	<CENTER> Вариант 8 </CENTER>
 * Вариант 8. Используя паттерн Builder, реализовать процесс создания объекта типа «Пицца» разных видов.
 */



#include <iostream>
#include <vector>

using namespace std;


// Сначала объявим продукты (классы всех возможных продуктов) которые используются для приготовления пиццы

/*!
	\brief Тесто

	Ингредиент
*/

class dough  
{
public:
void info() {
cout << "Dough" << endl;
}
};
/*!
	\brief колбаса пепперони

	Ингредиент
*/

class sausage_pepperoni
{
public:
void info() {
cout << "Sausage pepperoni" << endl;
}
};
/*!
	\brief Сыр моцарелла  

	Ингредиент
*/
class cheese_Mozzarella
{
public:
void info() {
cout << "Mozzarella" << endl;
}
};
/*!
	\brief Томатная паста

	Ингредиент
*/

class tomato_paste
{
public:
void info() {
cout << "Tomato paste" << endl;
}
};
/*!
	\brief Оливковое масло

	Ингредиент
*/

class olive_oil
{
public:
void info() {
cout << "Olive oil" << endl;
}
};
/*!
	\brief Яйца

	Ингредиент
*/

class egg
{
public:
void info() {
cout << "Egg" << endl;
}
};
/*!
	\brief Помидор

	ингредиент
*/
class tomatoes
{
public:
void info() {
cout << "Tomatoes" << endl;
}
};


/*!
	\brief Класс пицца

	Содержит все возможные ингредиенты 
*/


class Pizza
{
public:
vector <dough> doug;
vector <sausage_pepperoni> papperoni;
vector <cheese_Mozzarella> mozzarel ;
vector <tomato_paste> tomato;
vector <olive_oil> oill;
vector <egg> eggg;
vector <tomatoes> tomatoe;
void info() {
int i;
int counter=1;
for(i=0; i<doug.size(); ++i) {cout<<counter <<") "; doug[i].info();  counter++;}
for(i=0; i<papperoni.size(); ++i) {cout<<counter <<") "; papperoni[i].info(); counter++;}
for(i=0; i<mozzarel.size(); ++i) {cout<<counter <<") "; mozzarel[i].info(); counter++;}
for(i=0; i<tomato.size(); ++i) {cout<<counter <<") "; tomato[i].info(); counter++;}
for(i=0; i<oill.size(); ++i) {cout<<counter <<") "; oill[i].info(); counter++;}
for(i=0; i<eggg.size(); ++i) {cout<<counter <<") "; eggg[i].info();	counter++;}
for(i=0; i<tomatoe.size(); ++i) {cout<<counter <<") "; tomatoe[i].info(); counter++;}		

}
};

/*!
	\brief Базовый класс
	Базовый класс который объявляет интерфейс для поэтапного построения пиццы и предусматривает его реализацию по умолчанию	 
*/

class Pizza_Builder
{
protected:
Pizza* p;
public:
Pizza_Builder(): p(0) {}
virtual ~Pizza_Builder() {}
virtual void createPizza() {}
virtual void build_doug() {}
virtual void build_papperoni() {}
virtual void build_mozzarel() {}
virtual void build_tomato() {}
virtual void build_oill() {}
virtual void build_egg() {}
virtual void build_tomatoe() {}

virtual Pizza* getPizza() { return p; }
};


/*!
	\brief Пепперони 
	
	Состоит из: 1) Тесто 2) Колбаса пепперони 3) Моцарелла 4) Томатная паста 5) Оливковое масло
*/

class Papperoni_Pizza_Builder: public Pizza_Builder
{
public:
void createPizza() { p = new Pizza; }
void build_doug() { p->doug.push_back( dough()); }
void build_papperoni() { p->papperoni.push_back( sausage_pepperoni()); }
void build_mozzarel() { p->mozzarel.push_back( cheese_Mozzarella()); }
void build_tomato() { p->tomato.push_back( tomato_paste()); }
void build_oill() { p->oill.push_back( olive_oil()); }
};

/*!
	\brief Маргарита 
	
	Состоит из: 1) Моцарелла 2) Оливковое масло 3) Томатная паста 4) Помидоры 5) Тесто
*/

class Margarita_Pizza_Builder: public Pizza_Builder
{
public:
void createPizza() { p = new Pizza; }
void build_doug() { p->doug.push_back( dough()); }
void build_mozzarel() { p->mozzarel.push_back( cheese_Mozzarella()); }
void build_tomato() { p->tomato.push_back( tomato_paste()); }
void build_oill() { p->oill.push_back( olive_oil()); }
void build_tomatoe() { p->tomatoe.push_back( tomatoes()); }
};


/*!
	\brief Класс-распорядитель
	
	Поэтапно создает пиццу определенного вида. 

	Тут мы можем найти алгорим построения пиццы.	
*/

class Director
{
public:
Pizza* createPizza( Pizza_Builder & builder )
{
	builder.createPizza();
	builder.build_doug();
	builder.build_papperoni();
	builder.build_mozzarel();
	builder.build_tomato();
	builder.build_oill();
	builder.build_tomatoe();
    builder.build_egg();


return( builder.getPizza());
}
};

/*!
	\brief главная функция 

	И тут происходит client часть
*/


int main()
{

Director dir;
Papperoni_Pizza_Builder PP_builder;
Margarita_Pizza_Builder MP_builder;
Pizza * Papperonii = dir.createPizza( PP_builder);
Pizza * Margaritaa = dir.createPizza( MP_builder);
cout << "Papperoni:" << endl;
Papperonii->info();
cout << "\nMorgarita:" << endl;
Margaritaa->info();


return 0;
}



