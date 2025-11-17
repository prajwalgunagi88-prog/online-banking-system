#include <stdio.h>

// Function to perform Binary Search
int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = (low + high) / 2;

        if (arr[mid] == key)
            return mid; // Element found
        else if (arr[mid] < key)
            low = mid + 1; // Search right half
        else
            high = mid - 1; // Search left half
    }
    return -1; // Element not found
}

int main() {
    int n, key, result;

    // Input array size
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];

    // Input sorted array elements
    printf("Enter %d elements in ascending order:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    // Input the element to search
    printf("Enter the element to search: ");
    scanf("%d", &key);

    // Call the binary search function
    result = binarySearch(arr, n, key);

    // Display result
    if (result == -1)
        printf("Element not found in the array.\n");
    else
        printf("Element found at position %d.\n", result + 1);

    return 0;
}