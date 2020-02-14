#include "Dinic_graph.h"
bool Dinic_Graph::Breadth_First_Search()
{
	std::deque<uint> queue; // очередь индексов вершин графа
	for (unsigned j = 0; j < V; j++)
	{
		graph[j].dist = 0; // дистанции всех вершин изначально не определены
	}
	graph[source].dist = 1; // дистанция источника = 1
	queue.push_back(source); // помещаем индекс вершины-источника в начало очереди
	while (queue.size() != 0 && graph[sink].dist == 0) //пока в очереди есть вершины или пока не посетили вершину-сток															
	{
		for (std::list<edge>::iterator it = graph[queue.front()].edges.begin(); it != graph[queue.front()].edges.end(); it++)
		{
			if ((graph[it->r].dist == 0) && (it->cur < it->max))
			//если рассматриваемое ребро текущей вершины ведет в вершину, которую мы не посещали (dist == -1), и пропускная способность текущего ребра не исчерпана
			{
				graph[it->r].dist = graph[queue.front()].dist + 1;// присваивание вершине, в которую ведет ребро, дистанции, на 1 больше дистанции текущей вершины
				queue.push_back(it->r); // добавление индекса этой вершины в конец очереди
			}
		}
		queue.pop_front(); // удаление текущей вершины из начала очереди
	}
	return graph[sink].dist != 0; // если не дошли до вершины стока (ее дистанция == -1) возврат false
}
n_type Dinic_Graph::Depth_First_Search(uint cur_vert, n_type flow) // на каждом шаге рекурсии мы знаем текущую вершину и поток, который она получает из предыдущих вершин
{
	if (flow == 0) // если вершина приняла нулевой поток
	{
		return 0; // то и возвращаем 0 
	}
	if (cur_vert == sink) // если мы попали в вершину-сток
	{
		return flow; // то возвращаем величину потока, которую приняли
	}
	n_type pushed; // величина потока, поторую мы получаем из рекурсивного вызова
	for (std::list<edge>::iterator it = graph[cur_vert].edges.begin(); it != graph[cur_vert].edges.end(); it++)
	{
		if (graph[it->r].dist != graph[cur_vert].dist + 1)
			// если ребро ведет в вершину, дистанция которой не равна дистанции текущей вершины + 1
		{
			continue; // то это ребро мы рассматривать не будем
		}
		pushed = Depth_First_Search(it->r, min(flow, (it->max - it->cur)));
			//рекурсивный вызов: в качестве текущей вершины передаем номер вершины-конца текущего ребра, в качестве потока передаем минимум из двух величин (величина принятого потока, оставшаяся пропускная способность текущего ребра)
		if (pushed != 0) // если получили ненулевую величину потока
		{
			it->cur += pushed; // добавляем к текущему потоку текущего ребра полученную величину
			it->rev->cur -= pushed; // и вычитаем эту величину из ребра, обратного текущему
			return pushed; // возвращаем величину дальше
		}
	}
	return 0;
}
Dinic_Graph::Dinic_Graph(n_type infinity, std::istream& input)
{
	INF = infinity;
	graph = nullptr;
	input >> (*this);
}
Dinic_Graph::~Dinic_Graph()
{
	if (graph != nullptr)
	{
		delete[] graph;
	}
}
Dinic_Graph::Dinic_Graph(const Dinic_Graph& obj)
{
	INF = obj.INF;
	graph = nullptr;
	if (obj.graph != nullptr)
	{
		V = obj.V;
		source = obj.source;
		sink = obj.sink;
		graph = new vertex[V];
		for (uint j = 0; j < V; j++)
		{
			for (std::list<edge>::iterator it = obj.graph[j].edges.begin(); it != obj.graph[j].edges.end(); it++)
			{
				if (it->max > 0)// действия будут проводиться только с прямыми ребрами (у обратных всегда max == 0)
				{
					graph[j].edges.push_back(*it);//добавление текущего ребра
					graph[it->r].edges.push_back(*(it->rev)); // добавление ребра, обратного текущему

					graph[j].edges.back().rev = &graph[it->r].edges.back();//обновление указателей
					graph[it->r].edges.back().rev = &graph[j].edges.back();//так как изначально указатели содержат адреса ребер в порождающем графе
				}
			}
		}
	}
}
Dinic_Graph& Dinic_Graph::operator=(const Dinic_Graph& rvl)
{
	if (this != &rvl)//проверка самоприсваивания
	{
		if (graph != nullptr)
		{
			delete[] graph;
		}
		INF = rvl.INF;
		graph = nullptr;
		if (rvl.graph != nullptr)
		{
			V = rvl.V;
			source = rvl.source;
			sink = rvl.sink;
			graph = new vertex[V];
			for (uint j = 0; j < V; j++)
			{
				for (std::list<edge>::iterator it = rvl.graph[j].edges.begin(); it != rvl.graph[j].edges.end(); it++)
				{
					if (it->max > 0)// действия будут проводиться только с прямыми ребрами (у обратных всегда max == 0)
					{
						graph[j].edges.push_back(*it);//добавление текущего ребра
						graph[it->r].edges.push_back(*(it->rev)); // добавление ребра, обратного текущему

						graph[j].edges.back().rev = &graph[it->r].edges.back();//обновление указателей
						graph[it->r].edges.back().rev = &graph[j].edges.back();//так как изначально указатели содержат адреса ребер в порождающем графе
					}
				}
			}
		}

	}
	return *this;
}
n_type Dinic_Graph::Find_Max_Flow(std::ostream& out)
{
	n_type rezult = 0;
	uint pushed;
	while (Breadth_First_Search()) // продолжаем пока можно из истока дойти до стока
	{
		do
		{
			pushed = Depth_First_Search(source, INF); // идем из истока в сток (один путь), принимаемая истоком величина потока считается бесконечной
			rezult = rezult + pushed;
		} while (pushed != 0); // до тех пор, пока путь можно найти (следовательно передать хотя бы единицу потока) с текущей расстановкой дистанций
		out << *this;// вывод текущего состояния графа
	}
	return rezult;
}
std::istream& operator>>(std::istream& in, Dinic_Graph& obj)
{
	if (obj.graph != nullptr)//случай повторного использования объекта для другого графа
	{
		delete[] obj.graph;
	}
	uint E; // количество строк, содержащих ребра, используется локально
	in >> obj.V >> E >> obj.source >> obj.sink;
	obj.graph = new Dinic_Graph::vertex[obj.V]; // массив вершин из V элементов
	Dinic_Graph::edge e_temp; // используется для ввода и сохранения очередного ребра (прямого)
	Dinic_Graph::edge e_rev;  // используется для сохранения ребра, обратного введенному
	e_temp.cur = 0; // изначально текущий поток во всех ребрах=0
	e_rev.cur = 0; e_rev.max = 0;
	for (uint j = 0; j < E; j++)
	{
		in >> e_temp.l >> e_temp.r >> e_temp.max;//ввод очередного ребра
		//
		e_rev.l = e_temp.r;// присваивание полей для обратного ребра
		e_rev.r = e_temp.l;// при этом текущий и максимальный поток обратного ребра при вводе графа по определению = 0 (строка 89)
		//
		obj.graph[e_temp.l].edges.push_back(e_temp);//добавление введенного ребра в список ребер вершины-начала ребра
		obj.graph[e_temp.r].edges.push_back(e_rev);// обратное ребро добавляется в список ребер вершины-конца прямого ребра
		//
		obj.graph[e_temp.l].edges.back().rev = &obj.graph[e_temp.r].edges.back();// установление связи с помощью указателей
		obj.graph[e_temp.r].edges.back().rev = &obj.graph[e_temp.l].edges.back();// теперь ребра взаимнообратны
	}
	return in;
}
std::ostream& operator<<(std::ostream& out, const Dinic_Graph& obj)// потоковый вывод текущего состояния графа
{
	for (unsigned int i = 0; i < obj.V; i++)
	{
		out << std::endl << "v" << i << "      dist = " << obj.graph[i].dist << std::endl << std::endl;
		for (std::list<Dinic_Graph::edge>::iterator it = obj.graph[i].edges.begin(); it != obj.graph[i].edges.end(); it++)
		{
			out << "   " << it->l << " --> " << it->r << "    " << it->cur << " / " << it->max << std::endl;
		}
	}
	out << std::endl << "///////////////////////////" << std::endl;
	return out;
}