# TIME : 2023/12/08 14:01

from typing import Optional, cast


class DistanceVector:
    def __init__(self, graph):
        self.graph = graph
        self.num_nodes = len(graph)
        self.INFINITY = float('inf')
        self.distance_table = [[self.INFINITY] * self.num_nodes for _ in range(self.num_nodes)]
        self.next_hop_table = [[None] * self.num_nodes for _ in range(self.num_nodes)]

    def initialize(self):
        # 初始化距离表和下一跳表
        for i in range(self.num_nodes):
            for j in range(self.num_nodes):
                self.distance_table[i][j] = self.graph[i][j]
                if i != j and self.graph[i][j] != self.INFINITY:
                    self.next_hop_table[i][j] = cast(Optional[int], i)

    def relax(self, source, destination, intermediate):
        # 松弛操作，检查是否存在更短的路径
        if self.distance_table[source][destination] > self.distance_table[source][intermediate] + self.distance_table[intermediate][destination]:
            self.distance_table[source][destination] = self.distance_table[source][intermediate] + self.distance_table[intermediate][destination]
            self.next_hop_table[source][destination] = intermediate

    def distance_vector_algorithm(self):
        # 距离矢量算法的主循环
        for _ in range(self.num_nodes):  # 迭代次数
            for source in range(self.num_nodes):
                for destination in range(self.num_nodes):
                    for intermediate in range(self.num_nodes):
                        self.relax(source, destination, intermediate)

    def print_results(self, start_node):
        # 输出结果
        print(f"Distance from node {start_node} to other nodes:")
        distance_dict = {i: self.distance_table[start_node][i] for i in range(self.num_nodes)}
        print(distance_dict)

        print("\nShortest paths:")
        for destination in range(self.num_nodes):
            if start_node != destination:
                path = self.get_path(start_node, destination)
                if path:
                    path_str = " -> ".join(map(str, path))
                    print(f"Path from node {start_node} to node {destination}: {path_str}")
                else:
                    print(f"No path from node {start_node} to node {destination}")

        # 输出各节点的距离
        for destination in range(self.num_nodes):
            print(f"Distance from node {start_node} to node {destination}: {distance_dict[destination]}")

        print("\n" + "=" * 30)

    def get_path(self, source, destination):
        # 获取路径
        path = []
        intermediate = destination
        while intermediate is not None:
            path.insert(0, intermediate)
            intermediate = self.next_hop_table[source][intermediate]
        return path


inf = float('inf')
graph = [
    [0.0,   1.0,    inf,    6.0,    inf,    inf],
    [1.0,   0.0,    3.0,    4.0,    inf,    inf],
    [inf,   3.0,    0.0,    2.0,    6.0,    inf],
    [6.0,   4.0,    2.0,    0.0,    9.0,    2.0],
    [inf,   inf,    6.0,    9.0,    0.0,    inf],
    [inf,   inf,    inf,    2.0,    inf,    0.0]
]

dv = DistanceVector(graph)
dv.initialize()
dv.distance_vector_algorithm()

for start_node in range(dv.num_nodes):
    dv.print_results(start_node)
