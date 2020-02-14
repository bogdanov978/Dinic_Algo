#include "Dinic_graph.h"
bool Dinic_Graph::Breadth_First_Search()
{
	std::deque<uint> queue; // ������� �������� ������ �����
	for (unsigned j = 0; j < V; j++)
	{
		graph[j].dist = 0; // ��������� ���� ������ ���������� �� ����������
	}
	graph[source].dist = 1; // ��������� ��������� = 1
	queue.push_back(source); // �������� ������ �������-��������� � ������ �������
	while (queue.size() != 0 && graph[sink].dist == 0) //���� � ������� ���� ������� ��� ���� �� �������� �������-����															
	{
		for (std::list<edge>::iterator it = graph[queue.front()].edges.begin(); it != graph[queue.front()].edges.end(); it++)
		{
			if ((graph[it->r].dist == 0) && (it->cur < it->max))
			//���� ��������������� ����� ������� ������� ����� � �������, ������� �� �� �������� (dist == -1), � ���������� ����������� �������� ����� �� ���������
			{
				graph[it->r].dist = graph[queue.front()].dist + 1;// ������������ �������, � ������� ����� �����, ���������, �� 1 ������ ��������� ������� �������
				queue.push_back(it->r); // ���������� ������� ���� ������� � ����� �������
			}
		}
		queue.pop_front(); // �������� ������� ������� �� ������ �������
	}
	return graph[sink].dist != 0; // ���� �� ����� �� ������� ����� (�� ��������� == -1) ������� false
}
n_type Dinic_Graph::Depth_First_Search(uint cur_vert, n_type flow) // �� ������ ���� �������� �� ����� ������� ������� � �����, ������� ��� �������� �� ���������� ������
{
	if (flow == 0) // ���� ������� ������� ������� �����
	{
		return 0; // �� � ���������� 0 
	}
	if (cur_vert == sink) // ���� �� ������ � �������-����
	{
		return flow; // �� ���������� �������� ������, ������� �������
	}
	n_type pushed; // �������� ������, ������� �� �������� �� ������������ ������
	for (std::list<edge>::iterator it = graph[cur_vert].edges.begin(); it != graph[cur_vert].edges.end(); it++)
	{
		if (graph[it->r].dist != graph[cur_vert].dist + 1)
			// ���� ����� ����� � �������, ��������� ������� �� ����� ��������� ������� ������� + 1
		{
			continue; // �� ��� ����� �� ������������� �� �����
		}
		pushed = Depth_First_Search(it->r, min(flow, (it->max - it->cur)));
			//����������� �����: � �������� ������� ������� �������� ����� �������-����� �������� �����, � �������� ������ �������� ������� �� ���� ������� (�������� ��������� ������, ���������� ���������� ����������� �������� �����)
		if (pushed != 0) // ���� �������� ��������� �������� ������
		{
			it->cur += pushed; // ��������� � �������� ������ �������� ����� ���������� ��������
			it->rev->cur -= pushed; // � �������� ��� �������� �� �����, ��������� ��������
			return pushed; // ���������� �������� ������
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
				if (it->max > 0)// �������� ����� ����������� ������ � ������� ������� (� �������� ������ max == 0)
				{
					graph[j].edges.push_back(*it);//���������� �������� �����
					graph[it->r].edges.push_back(*(it->rev)); // ���������� �����, ��������� ��������

					graph[j].edges.back().rev = &graph[it->r].edges.back();//���������� ����������
					graph[it->r].edges.back().rev = &graph[j].edges.back();//��� ��� ���������� ��������� �������� ������ ����� � ����������� �����
				}
			}
		}
	}
}
Dinic_Graph& Dinic_Graph::operator=(const Dinic_Graph& rvl)
{
	if (this != &rvl)//�������� ����������������
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
					if (it->max > 0)// �������� ����� ����������� ������ � ������� ������� (� �������� ������ max == 0)
					{
						graph[j].edges.push_back(*it);//���������� �������� �����
						graph[it->r].edges.push_back(*(it->rev)); // ���������� �����, ��������� ��������

						graph[j].edges.back().rev = &graph[it->r].edges.back();//���������� ����������
						graph[it->r].edges.back().rev = &graph[j].edges.back();//��� ��� ���������� ��������� �������� ������ ����� � ����������� �����
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
	while (Breadth_First_Search()) // ���������� ���� ����� �� ������ ����� �� �����
	{
		do
		{
			pushed = Depth_First_Search(source, INF); // ���� �� ������ � ���� (���� ����), ����������� ������� �������� ������ ��������� �����������
			rezult = rezult + pushed;
		} while (pushed != 0); // �� ��� ���, ���� ���� ����� ����� (������������� �������� ���� �� ������� ������) � ������� ������������ ���������
		out << *this;// ����� �������� ��������� �����
	}
	return rezult;
}
std::istream& operator>>(std::istream& in, Dinic_Graph& obj)
{
	if (obj.graph != nullptr)//������ ���������� ������������� ������� ��� ������� �����
	{
		delete[] obj.graph;
	}
	uint E; // ���������� �����, ���������� �����, ������������ ��������
	in >> obj.V >> E >> obj.source >> obj.sink;
	obj.graph = new Dinic_Graph::vertex[obj.V]; // ������ ������ �� V ���������
	Dinic_Graph::edge e_temp; // ������������ ��� ����� � ���������� ���������� ����� (�������)
	Dinic_Graph::edge e_rev;  // ������������ ��� ���������� �����, ��������� ����������
	e_temp.cur = 0; // ���������� ������� ����� �� ���� ������=0
	e_rev.cur = 0; e_rev.max = 0;
	for (uint j = 0; j < E; j++)
	{
		in >> e_temp.l >> e_temp.r >> e_temp.max;//���� ���������� �����
		//
		e_rev.l = e_temp.r;// ������������ ����� ��� ��������� �����
		e_rev.r = e_temp.l;// ��� ���� ������� � ������������ ����� ��������� ����� ��� ����� ����� �� ����������� = 0 (������ 89)
		//
		obj.graph[e_temp.l].edges.push_back(e_temp);//���������� ���������� ����� � ������ ����� �������-������ �����
		obj.graph[e_temp.r].edges.push_back(e_rev);// �������� ����� ����������� � ������ ����� �������-����� ������� �����
		//
		obj.graph[e_temp.l].edges.back().rev = &obj.graph[e_temp.r].edges.back();// ������������ ����� � ������� ����������
		obj.graph[e_temp.r].edges.back().rev = &obj.graph[e_temp.l].edges.back();// ������ ����� ��������������
	}
	return in;
}
std::ostream& operator<<(std::ostream& out, const Dinic_Graph& obj)// ��������� ����� �������� ��������� �����
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