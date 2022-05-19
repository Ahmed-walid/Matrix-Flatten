#include <iostream>
#include<vector>
#include<tuple>
#include <stdexcept>
using namespace  std;

/*
 * Class Mat3 models 3D Matrices
 */
template<typename T>
class Mat3 {
private:
    vector<vector<vector<T>>> mat;
    int n,m,k;

public:
    // Constructor : Represents Matrix 3D
    Mat3(int n,int m,int k){
        if (n < 0 || m < 0 || k<0)
            throw std::invalid_argument("Mat3 arguments must be positive");
        mat.resize(n,vector<vector<T>>(m,vector<T>(k)));
        this->n = n;
        this->m = m;
        this->k = k;
    }

    // Returns the shape of the matrix 'n'*'m'*'k'
    tuple<int,int,int> getShape(){
        return make_tuple(n,m,k);
    }

    // Checks if the 'x','y','z' are valid indices
    bool isValidIndex(const int& x,const int& y, const int& z){
        if(x<0 || x>=n || y<0 || y>=m || z<0 || z>=k )
            return false;
        return true;
    }

    // Updates the location accessed by 'x','y','z' by 'val'
    bool setValueAt(const int& x,const int& y, const int& z,const int& val){
        if(isValidIndex(x,y,z)==false)
            return false;
        mat[x][y][z] = val;
        return true;
    }

    // Retrieve the value @ location accessed by 'x','y','z'
    pair<bool,int> getValueAt(const int& x,const int& y, const int& z){
        if(isValidIndex(x,y,z)==false)
            return {false,-1};  // operation is invalid

        return {true,mat[x][y][z]};
    }

};
/*
 * Class FlatMat3 models 3D matrices stored in 1D Vector.
 */
template<typename T>
class FlatMat3 {

private:
    vector<T> vec;
    int n,m,k;
public:

    // Constructor -  Represents 3D Matrices
    FlatMat3(int n,int m,int k){
        if (n < 0 || m < 0 || k<0)
            throw std::invalid_argument("FlatMat3 arguments must be positive");
        this->n=n;
        this->m=m;
        this->k=k;
        vec.resize(n*m*k);
    }

    // Constructor - Builds FlatMat3 object from Mat3 object.
    FlatMat3(Mat3<T> mat){
        tuple<int,int,int> shape = mat.getShape();
        this->n = get<0>(shape);
        this->m = get<1>(shape);
        this->k = get<2>(shape);
        vec.resize(n*m*k);
        for(int z=0;z<k;z++)
            for(int x=0;x<n;x++)
                for(int y=0;y<m;y++){
                    pair<int,int> ret = mat.getValueAt(x,y,z);
                    vec[flatIndex(x,y,z)] = ret.second;
                }
    }

    // Checks if the 'x','y','z' are valid indices
    bool isValidIndex(const int& x,const int& y, const int& z){
        if(x<0 || x>=n || y<0 || y>=m || z<0 || z>=k )
            return false;
        return true;
    }

    // Returns the shape of the matrix 'n'*'m'*'k'
    tuple<int,int,int> getShape(){
        return make_tuple(n,m,k);
    }

    // Converts the 3 indices 'x','y','z' into a single index to access data.
    int flatIndex(const int& x,const int& y, const int& z){
        if(isValidIndex(x,y,z)==false) return -1;
        return z*(n*m) + x*m + y;
    }

    // Updates FlatMatrix at certain location  by 'val'
    bool setValueAt(const int& x,const int& y, const int& z,const int& val){
        if(isValidIndex(x,y,z)== false)
            return false;
        vec[flatIndex(x,y,z)] = val;
        return true;
    }

    // Retrieve value stored at certain location in FlatMatrix.
    pair<bool,int> getValueAt(const int& x,const int& y, const int& z){
        if(isValidIndex(x,y,z)== false)
            return {false,-1};  // operation is invalid

        return {true,vec[flatIndex(x,y,z)]};
    }

    //Print FlatMatrix (1D Vector) values.
    void print(){
        cout<<endl;
        for(auto& x: vec)
            cout<<x<<" ";
        cout<<endl;
    }

};


int main() {

    int n,m,k;
    cout<<"Enter the dimensions of the 3D Matrix: \n";
    cout<<"Enter n: "; cin>>n;
    cout<<"Enter m: "; cin>>m;
    cout<<"Enter k: "; cin>>k; cout<<endl;

    Mat3<int> mat(n,m,k);
    cout<<"Enter the values of the matrix:\n";
    for(int z=0;z<k;z++)
        for(int x=0;x<n;x++)
            for(int y=0;y<m;y++)
            {
                cout<<"Enter the value at x = "<<x+1<<" y = "<<y+1<<" z = "<<z+1<<endl;
                int val; cin>>val;
                mat.setValueAt(x,y,z,val);
            }

    //////////////TEST1////////////////
    FlatMat3<int> flatMat (mat) ;
    cout<<"\nTest 1 (Flattening 3d Matrix): The Flatten Matrix Values:";
    flatMat.print();

    /////////////TEST2///////////////
    cout<<endl; cout<<"Test 2: O(1) Indexing\n";
    for(int z=0;z<k;z++)
        for(int x=0;x<n;x++)
            for(int y=0;y<m;y++) {
                cout<<"Value at x = "<<x+1<<" y = "<<y+1<<" z = "<<z+1<<endl;
                if(flatMat.getValueAt(x,y,z).first)
                    cout<<flatMat.getValueAt(x,y,z).second<<endl;
                else
                    cout<<"Bad Access!\n";
            }

    ////////////////TEST 3/////////////
    cout<<"\nSet new values in FlatMat3: (Multiply by 10 to all locations)\n";
    for(int z=0;z<k;z++)
        for(int x=0;x<n;x++)
            for(int y=0;y<m;y++) {
                cout<<"New value at x = "<<x+1<<" y = "<<y+1<<" z = "<<z+1<<endl;
                int newVal = flatMat.getValueAt(x,y,z).second * 10;
                flatMat.setValueAt(x,y,z,newVal);
                cout<<flatMat.getValueAt(x,y,z).second<<" \n";
            }
    return 0;
}
