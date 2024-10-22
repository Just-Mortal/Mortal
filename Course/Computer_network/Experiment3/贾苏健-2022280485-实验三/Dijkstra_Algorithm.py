# TIME : 2023/12/08 14:01

import heapq


def dijkstra(graph, start):
    # 初始化距离字典和路径列表
    distance = {node: float('infinity') for node in range(len(graph))}
    distance[start] = 0
    path = [None] * len(graph)

    # 使用优先队列（最小堆）来加速查找最短路径
    priority_queue = [(0, start)]

    while priority_queue:
        current_distance, current_node = heapq.heappop(priority_queue)

        # 如果当前距离比已知距离大，跳过
        if current_distance > distance[current_node]:
            continue

        # 更新相邻节点的距离和路径
        for neighbor, weight in enumerate(graph[current_node]):
            new_distance = current_distance + weight
            if new_distance < distance[neighbor]:
                distance[neighbor] = new_distance
                path[neighbor] = current_node
                heapq.heappush(priority_queue, (new_distance, neighbor))

    return distance, path


def get_path(start, end, path_list):
    # 通过递归获取路径
    if end == start:
        return f"{start}"
    else:
        return f"{get_path(start, path_list[end], path_list)} -> {end}"


inf = float('inf')
graph = [
    [0.0,   1.0,    inf,    6.0,    inf,    inf],
    [1.0,   0.0,    3.0,    4.0,    inf,    inf],
    [inf,   3.0,    0.0,    2.0,    6.0,    inf],
    [6.0,   4.0,    2.0,    0.0,    9.0,    2.0],
    [inf,   inf,    6.0,    9.0,    0.0,    inf],
    [inf,   inf,    inf,    2.0,    inf,    0.0]
]

num_nodes = len(graph)

for start_node in range(num_nodes):
    # 运行Dijkstra算法
    distance_list, path_list = dijkstra(graph, start_node)

    print(f"Distance from node {start_node} to other nodes:")
    print(distance_list)
    print("\nShortest paths:")
    for node, path in enumerate(path_list):
        # 输出路径时需要判断是否为起点，避免输出 None
        if path is not None:
            print(f"Path from node {start_node} to node {node}: {get_path(start_node, node, path_list)}")
        else:
            print(f"No path from node {start_node} to node {node}")

    for node, distance in enumerate(distance_list):
        if node != start_node:
            print(f"Distance from node {start_node} to node {node}:", distance_list[node])

    print("\n" + "=" * 30)
