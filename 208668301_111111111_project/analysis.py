import numpy as np
import sys
from symnmf import perform_symnmf
from kmeans import kmeansAlgo
from sklearn.metrics import silhouette_score

np.random.seed(0)

def load_data(filename):
    return np.loadtxt(filename, delimiter=',')

def assign_kmeans_labels(data, centroids):
    labels = []
    for point in data:
        distances = [np.linalg.norm(np.array(point) - np.array(centroid)) for centroid in centroids]
        labels.append(np.argmin(distances))
    return labels

def assign_symnmf_labels(decomposition_matrix):
    return [np.argmax(row) for row in decomposition_matrix]


if __name__ == "__main__":
    k = int(sys.argv[1])
    filename = sys.argv[2]
    
    data = load_data(filename)

    with open(filename, 'r') as f:
        X = [[float(x) for x in line.split(',')] for line in f]
    
    NUM_of_DP = len(X)
    COORDS = len(X[0])
    
    kmeans_output = kmeansAlgo(X, k)
    symnmf_output = perform_symnmf(X, k, COORDS, NUM_of_DP)

    kmeans_labels = assign_kmeans_labels(data, kmeans_output)
    symnmf_labels = assign_symnmf_labels(symnmf_output)

    kmeans_silhouette = silhouette_score(data, kmeans_labels)
    symnmf_silhouette = silhouette_score(data, symnmf_labels)

    print(f"nmf: {symnmf_silhouette:.4f}")
    print(f"kmeans: {kmeans_silhouette:.4f}")

