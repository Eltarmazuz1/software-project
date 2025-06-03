import math
import sys


def distance(point1, point2):
    """Compute the Euclidean distance between two points."""
    return math.sqrt(sum([(point1[i] - point2[i]) ** 2 for i in range(len(point1))]))


def kmeansAlgo(data_arr, k, max_iters=200):
    """Cluster the data in the given file into k clusters using the K-means algorithm."""
    N = len(data_arr)
    d = len(data_arr[0])
    EPS = 0.001

    centroids = [data_arr[i] for i in range(k)]

    # Run iterations
    for iter in range(max_iters):
        # Assign each data point to its closest centroid
        cluster_assignment = [None] * N
        for i in range(N):
            min_dist = math.inf
            for j in range(k):
                dist = distance(data_arr[i], centroids[j])
                if dist < min_dist:
                    min_dist = dist
                    cluster_assignment[i] = j

        # Update centroids to be the mean of the assigned data points
        cluster_counts = [0] * k
        new_centroids = [[0.0] * d for i in range(k)]
        for i in range(N):
            cluster = cluster_assignment[i]
            cluster_counts[cluster] += 1
            for j in range(d):
                new_centroids[cluster][j] += data_arr[i][j]
        for i in range(k):
            if cluster_counts[i] > 0:
                for j in range(d):
                    new_centroids[i][j] /= cluster_counts[i]

        # Check for convergence
        converged = True
        for i in range(k):
            if distance(new_centroids[i], centroids[i]) > EPS:
                converged = False
                break

        if converged:
            break

        centroids = new_centroids
    return centroids

#    print_result(centroids)


def main():
    user_input = check_valid_input()
    if user_input == 1:
        return 1
    else:
        k, max_iters, data_file = user_input
    
    # Read data from file
    with open(data_file, 'r') as f:
        data_arr = [[float(x) for x in line.split(',')] for line in f]
    
    N = len(data_arr)

    if (k <= 1 or k >= N):
        print("Invalid number of clusters!")
        return 1
    
    kmeansAlgo(data_arr, k, max_iters)

    return 0

    

def check_valid_input():
    try:
        k = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        return 1
    num_of_arguments = len(sys.argv)
    if num_of_arguments == 4:
        try:
            max_iters = int(sys.argv[2])
        except:
            print("Invalid maximum iteration!")
            return 1
        data_file = sys.argv[3]
        if (max_iters <= 1 or max_iters >= 1000):
            print("Invalid maximum iteration!")
            return 1    
    else:
        data_file = sys.argv[2]
        max_iters = 200

    return [k, max_iters, data_file]

def print_result(centoridsArray):
    for centroid in range(len(centoridsArray)):
        print(*["%0.4f" % i for i in centoridsArray[centroid]], sep=",")



if __name__ == "__main__":
    main()
