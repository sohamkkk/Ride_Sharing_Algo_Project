#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> Pair;
typedef pair<double,pair<int,int>> pPair;
//class to define cell and its parameters like coordinates of parents cells and costs
struct cell{

   int parent_i;
   int parent_j;
   double f,g,h;
};
//priority queue implemented as a min heap to find out minimum f value from 2 nodes in less time i.e. is logn
class PriorityQueue{
private:
   vector<pair<double,Pair>> heap;
   //function to maintain condition of min heap i.e.every node should be less than or equal to its descendent
   void heapifyup(int index){
    while(index>0&&heap[(index-1)/2].first>heap[index].first){
        swap(heap[index],heap[(index-1)/2]);
        index=(index-1)/2;

    }
   }
   //function to maintain heap property when an element is removed
   /*when an element is deleted the bottom element is shifted at top so min-heap condition is violated hence 
   be restored so we use this function
   */
   void heapifydown(int index){
    int smallest=index;
    int left=2*index+1;
    int right=2*index+2;
    if(left<heap.size()&&heap[left].first<heap[smallest].first){
        smallest=left;
    }
    if(right<heap.size()&&heap[index].first>heap[right].first){
        smallest=right;
    }
    if(smallest!=index){
        swap(heap[index],heap[smallest]);
        heapifydown(smallest);
    }

   }
public:
//function to push cost value and coordinates of cell into heap
   void push(pair<double,Pair> val){
    heap.push_back(val);
    heapifyup(heap.size()-1);
   }
   //function to remove the pair of cost value and coordinates of cell from min heap
   void pop(){
    heap[0]=heap.back();
    heap.pop_back();
    heapifydown(0);

   }
   //fucntiom to view the top element i.e. min element of heap
   pair<double,Pair>> top(){
    if(!heap.empty()) return heap[0];
    return {FLT_MAX,{-1,-1}};//return invalid if empty


   }
   //function to check if heap is empty or not
   bool empty(){
    
    if(heap.empty()) return true;
    return false;
   }


};
class ASTARSEARCHALGORITHM{
public:
   int ROW,COL;
   ASTARSEARCHALGORITHM(int row,int col):ROW(row),COL(col){}

   //function to check that coordinates of cells are valid or not
   bool isvalid(int row,int col){
    if(row>=0&&row<ROW&&col>=0&&col<COL) return true;
    return false;
   }
   //function to check if a given cell is unblocked or not,1-unblocked(true returned),0-blocked(false returned)
   bool isunblocked(int row,int col,const vector<vector<int>>&grid){
    if(grid[row][col]==1) return true;
    else return false;
   }
   //functon to check if we have reached destination or not
   bool isdestination(int row,int col,Pair dest){
    if(row==dest.first&&col==dest.second) return true;
    else return false;
   }
   //function to calculate heuristic that is h value using distance formula
   double calculatehvalue(int row,int col,Pair dest){
     return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
   }
   //function to trace path from source cell tpo destination cell
   void tracepath(cell celldetails[][100],Pair dest){
    cout<<"\nthe path is";
    //initialse variables to coordinates of destination cell
    int i=dest.first;
    int j=dest.second;
    //use stack to store coordinates of cells 
    stack<Pair> path;
    //a loop that runs till we reach source cell
    //parent coordinates of source cell are source coordinates itself
    while(!(celldetails[i][j].parent_i==i&&celldetails[i][j].parent_j==j)){
        path.push(make_pair(i,j));
        int temp_i=celldetails[i][j].parent_i;
        int temp_j=celldetails[i][j].parent_j;
        i=temp_i;
        j=temp_j;
    }
    //push coordinates of source cell into stack
    path.push(make_pair(i,j));
    //print all values of stack from top to bottom to generate the shortest part
    while(!path.empty()){
        pair<int,int> p=path.top();
        path.pop();
        cout<<"("<<p.first<<","<<p.second<<")";
        if(!path.empty())cout<<"->";


    }
    cout<<endl;
    return;

    
   }
   //function to implemet astarsearch to find shortest path
   void astareach(const vector<vector<int>>&grid,Pair src,Pair dest){
    //to check if source and destination coordinates are valid or not
    if(!isvalid(src.first,src.second)){
        cout<<"source is invalid"<<endl;
        return;
    }
    if(!isvalid(dest.first,dest.second)){
        cout<<"destination is invalid"<<endl;
        return;
    }
    //to check is source or destination are unblocked
    if(!isunblocked(src.first,src.second,grid)){
        cout<<"source is blocked"<<endl;
        return;
    }
    if(!isunblocked(dest.first,dest.second,grid)){
        cout<<"destination is blocked"<<endl;
        return;
    }
    //check is source is destination or not
    if(isdestination(src.first,src.second,dest)){
        cout<<"we already are at the destination"<<endl;
        return;

    }
    //create a closed list to keep track which cells have been evalauted
    //list is intialised to false meaning no cell has been evalauted
    //it is of bool type

    //closed list hepls to avoid cells that have been already evaluated 
    //improves effficiency
    vector<vector<bool>> closedlist(ROW,vector<bool>(COL,false));
    //declaring a 2d array of class cell to hold details of every cell
    cell celldetails[100][100];
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            celldetails[i][j].parent_i=-1;
            celldetails[i][j].parent_j=-1;
            celldetails[i][j].f=FLT_MAX;
            celldetails[i][j].g=FLT_MAX;
            celldetails[i][j].h=FLT_MAX;

        }
    }
    //initialise parameters of source node
    i=src.first;
    j=src.second;
    celldetails[i][j].parent_i=i;
    celldetails[i][j].parent_j=j;
    celldetails[i][j].f=0;
    celldetails[i][j].g=0;
    celldetails[i][j].h=0;
    /*creating an open list that contains cells for evaluation
    contains cells that are candidates for evaluation 
    cell with least f value will be selected 
    each cell has maximum 8 neighbours that can be evaluated 
    */
   //priority queue is used to implement openlist i.e. min-heap as it hepls to find element in logn time
   PriorityQueue openlist;
    
    
    //put the source cell on open list and initialise its f as zero
    openlist.push({0,{i,j}});
    //variable is defined to check if final destination is reached or not
    bool found_dest=false;
    while(!openlist.empty()){
        auto p=openlist.top();
        //removing this coordinate
        openlist.pop();
        i=p.second.first;
        j=p.second.second;
        //cell is under evalaution so change its bool value in closed list
        closedlist[i][j]=true;
        
        //generate 8 successors of this cell
        /*
        N-(i,j+1)
        S-(i,j-1)
        W-(i-1,j)
        E-(i+1,j)
        NW-(i-1,j+1)
        NE-(i+1,j+1)
        SW-(i-1,j-1)
        SE-(i+1,j-1)
        */
       //declare f,g,h cost value of successor cells
       double hnew,gnew,fnew;
       //1st Successor-(i,j+1)
       //only continue if cell is valid one
       if(isvalid(i,j+1)){
        //if successor is same as destination
        if(isdestination(i,j+1,dest)){
            //set successors parents coordinates to i and j
            celldetails[i][j+1].parent_i=i;
            celldetails[i][j+1].parent_j=j;
            cout<<"destination cell is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        //if successor is not on closed list
        //and it is unblocked 
        else if(closedlist[i][j+1]==false&&isunblocked(i,j+1,grid)){
            gnew=celldetails[i][j].g+1;
            hnew=calculatehvalue(i,j+1,dest);
            fnew=gnew+hnew;
            //if successor isnt on open list
            //add it to open list
            //set cell as its parents and update cost values
            //if successor is in open list
            //check if it is better path to destination
            //compare f value and fnew
            if(celldetails[i][j+1].f==FLT_MAX||celldetails[i][j+1].f>fnew){
                //inserting to open list
                openlist.push(make_pair(fnew,make_pair(i,j+1)));
                 //update its details
                celldetails[i][j+1].g=gnew;
                celldetails[i][j+1].h=hnew;
                celldetails[i][j+1].f=fnew;
                celldetails[i][j+1].parent_i=i;
                celldetails[i][j+1].parent_j=j;

            }
        }
       }
       //repeat for remaining 7 successors
       //2nd successor-(i,j-1)
       //check for valid
       if(isvalid(i,j-1)){
        //if same as destination cell or not
        if(isdestination(i,j-1,dest)){
            celldetails[i][j-1].parent_i=i;
            celldetails[i][j-1].parent_j=j;
            cout<<"destination cell is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        //if unblocked and not on closedlist
        else if(closedlist[i][j-1]==false&&isunblocked(i,j-1,grid)){
            gnew=celldetails[i][j].g+1;
            hnew=calculatehvalue(i,j-1,dest);
            fnew=gnew+hnew;
            //if not on openlist or fnew<f
            if(celldetails[i][j-1].f==FLT_MAX||celldetails[i][j-1].f>fnew){
                openlist.insert(make_pair(fnew,make_pair(i,j-1)));
                celldetails[i][j-1].g=gnew;
                celldetails[i][j-1].h=hnew;
                celldetails[i][j-1].f=fnew;
                celldetails[i][j-1].parent_i=i;
                celldetails[i][j-1].parent_j=j;

            }

        }
       }
       //3rd successor-(i-1,j)
       if(isvalid(i-1,j)){
        if(isdestination(i-1,j,dest)){
            celldetails[i-1][j].parent_i=i;
            celldetails[i-1][j].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;


        }
        else if(closedlist[i-1][j]==false&&isunblocked(i-1,j,grid)){
            gnew=celldetails[i][j].g+1;
            hnew=calculatehvalue(i-1,j,dest);
            fnew=gnew+hnew;
            if(celldetails[i-1][j].f==FLT_MAX||celldetails[i-1][j].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i-1,j)));
                celldetails[i-1][j].g=gnew;
                celldetails[i-1][j].h=hnew;
                celldetails[i-1][j].f=fnew;
                celldetails[i-1][j].parent_i=i;
                celldetails[i-1][j].parent_j=j;
  
            }

        }
       
       }
       //4th successor-(i+1,j)
       if(isvalid(i+1,j)){
        if(isdestination(i+1,j,dest)){
            celldetails[i+1][j].parent_i=i;
            celldetails[i+1][j].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        else if(closedlist[i+1][j]==false&&isunblocked(i+1,j,grid)){
            gnew=celldetails[i][j].g+1;
            hnew=calculatehvalue(i+1,j,dest);
            fnew=gnew+hnew;
            if(celldetails[i+1][j].f==FLT_MAX||celldetails[i+1][j].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i+1,j)));
                celldetails[i+1][j].g=gnew;
                celldetails[i+1][j].h=hnew;
                celldetails[i+1][j].f=fnew;
                celldetails[i+1][j].parent_i=i;
                celldetails[i+1][j].parent_j=j;

            }
        }
       
       }
       //5th successor-(i-1,j-1)
       if(isvalid(i-1,j-1)){
        if(isdestination(i-1,j-1,dest)){
            celldetails[i-1][j-1].parent_i=i;
            celldetails[i-1][j-1].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        else if(closedlist[i-1][j-1]==false&&isunblocked(i-1,j-1,grid)){
            gnew=celldetails[i][j].g+sqrt(2);
            hnew=calculatehvalue(i-1,j-1,dest);
            fnew=gnew+hnew;
            if(celldetails[i-1][j-1].f==FLT_MAX||celldetails[i-1][j-1].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i-1,j-1)));
                celldetails[i-1][j-1].g=gnew;
                celldetails[i-1][j-1].h=hnew;
                celldetails[i-1][j-1].f=fnew;
                celldetails[i-1][j-1].parent_i=i;
                celldetails[i-1][j-1].parent_j=j;
            }
        }
       
       }
       //6th successor-(i-1,j+1)

       if(isvalid(i-1,j+1)){

        if(isdestination(i-1,j+1,dest)){
            celldetails[i-1][j+1].parent_i=i;
            celldetails[i-1][j+1].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        else if(closedlist[i-1][j+1]==false&&isunblocked(i-1,j+1,grid)){
            gnew=celldetails[i][j].g+sqrt(2);
            hnew=calculatehvalue(i-1,j+1,dest);
            fnew=gnew+hnew;
            if(celldetails[i-1][j+1].f==FLT_MAX||celldetails[i-1][j+1].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i-1,j+1)));
                celldetails[i-1][j+1].g=gnew;
                celldetails[i-1][j+1].h=hnew;
                celldetails[i-1][j+1].f=fnew;
                celldetails[i-1][j+1].parent_i=i;
                celldetails[i-1][j+1].parent_j=j;

            }
        }
       
       }
       //7th successor-(i+1,j+1)
       if(isvalid(i+1,j+1)){
        if(isdestination(i+1,j+1,dest)){
            celldetails[i+1][j+1].parent_i=i;
            celldetails[i+1][j+1].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        else if(closedlist[i+1][j+1]==false&&isunblocked(i+1,j+1,grid)){
            gnew=celldetails[i][j].g+sqrt(2);
            hnew=calculatehvalue(i+1,j+1,dest);
            fnew=gnew+hnew;
            if(celldetails[i+1][j+1].f==FLT_MAX||celldetails[i+1][j+1].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i+1,j+1)));
                celldetails[i+1][j+1].g=gnew;
                celldetails[i+1][j+1].h=hnew;
                celldetails[i+1][j+1].f=fnew;
                celldetails[i+1][j+1].parent_i=i;
                celldetails[i+1][j+1].parent_j=j;
            }
        }
      
       }
       //8th successor-(i+1,j-1)
       if(isvalid(i+1,j-1)){
        if(isdestination(i+1,j-1,dest)){
            celldetails[i+1][j-1].parent_i=i;
            celldetails[i+1][j-1].parent_j=j;
            cout<<"destination is found"<<endl;
            tracepath(celldetails,dest);
            found_dest=true;
            return;
        }
        else if(closedlist[i+1][j-1]==false&&isunblocked(i+1,j-1,grid)){
            gnew=celldetails[i][j].g+sqrt(2);
            hnew=calculatehvalue(i+1,j-1,dest);
            fnew=gnew+hnew;
            if(celldetails[i+1][j-1].f==FLT_MAX||celldetails[i+1][j-1].f>fnew){
                openlist.push(make_pair(fnew,make_pair(i+1,j+1)));
                celldetails[i+1][j-1].g=gnew;
                celldetails[i+1][j-1].h=hnew;
                celldetails[i+1][j-1].f=fnew;
                celldetails[i+1][j-1].parent_i=i;
                celldetails[i+1][j-1].parent_j=j;

            }
            
        }
       
       }
    }
    if(found_dest==false){
        cout<<"failed to find destination cell"<<endl;
        return;
    }

   }

   void implement_a_star_search(vector<vector<int>>grid,Pair src,Pair dest){
    astareach(grid,src,dest);
    return;
   }



   
   

};
//main function
int main(){
    int ROW,COL;
    cin>>ROW;
    cin>>COL;
    vector<vector<int>>grid(ROW,vector<int>(COL));
    //1-cell unblocked
    //0-cell blocked
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++) cin>>grid[i][j];
    }
    int source_i,source_j;//coordinates of starting point
    int dest_i,dest_j;//coordinates of destination point
    cin>>source_i;
    cin>>source_j;
    cin>>dest_i;
    cin>>dest_j;
    Pair src=make_pair(source_i,source_j);// pair containing coordinates
    Pair dest=make_pair(dest_i,dest_j);
    ASTARSEARCHALGORITHM X(ROW,COL);

    X.implement_a_star_search(grid,src,dest);

    return 0;
    


}
