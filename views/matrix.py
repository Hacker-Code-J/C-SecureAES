import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

# If your code looked like this
# array = np.array([1, 2, 3], dtype=np.int)

# You can simply change it to
array = np.array([1, 2, 3], dtype=int)

# Or, if you need a specific size, use
array_64 = np.array([1, 2, 3], dtype=np.int64)

# Create a directed graph
G = nx.DiGraph()

# Add nodes for original and new data
original_nodes = [f"X_{i}" for i in range(16)]
new_nodes = [f"New X_{i}" for i in range(16)]
G.add_nodes_from(original_nodes, layer=0)
G.add_nodes_from(new_nodes, layer=1)

# Add edges with weights (coefficients)
for i in range(4):
    for j in range(4):
        for k in range(4):
            weight = transformation_matrix[i, k]
            if weight > 0:
                G.add_edge(f"X_{4*k+j}", f"New X_{4*i+j}", weight=weight)

# Position nodes in two layers
pos = {}
pos.update((node, (1, i)) for i, node in enumerate(original_nodes)) # layer 0
pos.update((node, (2, i)) for i, node in enumerate(new_nodes)) # layer 1

# Draw the graph
plt.figure(figsize=(12, 8))
nx.draw(G, pos, with_labels=True, node_color="lightblue", node_size=2000, 
        arrows=True, arrowstyle='-|>', arrowsize=20, font_size=10, 
        edge_color='gray', width=[G[u][v]['weight'] for u, v in G.edges()])

# Add edge labels (weights)
edge_labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='red')

plt.title("Data Flow Visualization in Matrix Transformation")
plt.axis('off')
plt.show()
