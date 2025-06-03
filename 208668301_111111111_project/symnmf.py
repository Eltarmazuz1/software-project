import numpy as np
import sys
import mysymnmf

np.random.seed(0)


def initialize_h(W, k):
    
    W_array = np.array(W)
    m = np.mean(W_array)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), (W_array.shape[0], k))
    return H


def perform_symnmf(X, k, COORDS, NUM_of_DP):

    W = compute_normalized_similarity_matrix(X, COORDS, NUM_of_DP)

    H = initialize_h(W, k)

    H = mysymnmf.symnmf_capi(H.tolist(), W, k, NUM_of_DP)  # passing H and W to the C extension
  
    return H


def compute_similarity_matrix(X, COORDS, NUM_of_DP):

    return mysymnmf.sym_capi(X, COORDS, NUM_of_DP)  # passing X to the C extension


def compute_diagonal_degree_matrix(X, COORDS, NUM_of_DP):
    return mysymnmf.ddg_capi(X, COORDS, NUM_of_DP)  # passing X to the C extension


def compute_normalized_similarity_matrix(X, COORDS, NUM_of_DP):
    return mysymnmf.norm_capi(X, COORDS, NUM_of_DP)  # passing X to the C extension


def main():
    
    if(len(sys.argv)==4):
        k = int(sys.argv[1])
        goal = sys.argv[2]
        file_name = sys.argv[3]
    
    else: 
        goal = sys.argv[1]
        file_name = sys.argv[2]
   
    # read input file 

    with open(file_name, 'r') as f:
        X = [[float(x) for x in line.split(',')] for line in f]
    
    NUM_of_DP = len(X)
    COORDS = len(X[0])

    # add check if N>k else "An Error Has Occurred"#
    
    if goal == 'symnmf':

        H = perform_symnmf(X, k, COORDS, NUM_of_DP)

        for row in range(len(H)):
            print(*["%0.4f" % i for i in H[row]], sep=",")
            
    elif goal == 'sym':
        
        A = compute_similarity_matrix(X, COORDS, NUM_of_DP)
        for row in range(len(A)):
            print(*["%0.4f" % i for i in A[row]], sep=",")


    elif goal == 'ddg':

        D = compute_diagonal_degree_matrix(X, COORDS, NUM_of_DP)
        
        for row in range(len(D)):
            print(*["%0.4f" % i for i in D[row]], sep=",")


    elif goal == 'norm':

        W = compute_normalized_similarity_matrix(X, COORDS, NUM_of_DP)

        for row in range(len(W)):
            print(*["%0.4f" % i for i in W[row]], sep=",")


if __name__ == "__main__":
    main()
