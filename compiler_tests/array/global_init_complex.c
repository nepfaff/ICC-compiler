int arr[3][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };

char cArr[3][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };

float fArr[3][4] = {
        {1.1f,2.2f,3.3f,4.3f},
        {5.3f,6.3f,7.2f,8.1f},
        {9.3f,10.3f,11.3f,12.1f}
    };

double dArr[3][4] = {
        {1.0,2.0,3.0,4.0},
        {5.0,6.0,7.0,8.0},
        {9.0,10.0,11.0,12.0}
    };

unsigned int uArr[3][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };

int f() {
    return  (arr[1][2] == 7) &&
            (uArr[0][3] == 4) &&
            (cArr[2][3] == 12) &&
            (fArr[1][3] == 8.1f) &&
            (dArr[0][0] == 1.0);
}