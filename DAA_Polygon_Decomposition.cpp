/*Group details:
Abhiraj Khare 2020A7PS0161H
Shubhankar Vivek Shastri 2020A7PS2054H
Tanmay Agarwal 2020A7PS2057H
*/



#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct Vertex;
struct Halfedge;
struct Face;
vector<pair<int, int>> ans;
ifstream fp1;
ofstream fp2;
ofstream fp3;

/// @brief This struct contains x and y coordinated for a vertex that is to be added in the Vertex vector
struct Vertex {
    double x, y;
    Halfedge* leaving;
};

/// @brief This struct contains origin,twin,next and previous for a halfedge that is to be added in the halfedge vector
struct Halfedge {
    Vertex* origin;
    Halfedge* twin;
    Face* incident_face;
    Halfedge* next;
    Halfedge* prev;
};

/// @brief This struct contains a edge corresponding to a face that is to be added in the face vector
struct Face {
    Halfedge* edge;
};

//Implementation of DCEL
class DCEL {

public:    //store input vertices and edges in form of DCEL using structs vertex and Halfedge and adding connections between edges
    vector<Vertex*> vertices;
    vector<Halfedge*> halfedges;
    vector<Face*> faces;

    /// @brief This function adds vertices provided by user in a vector named 'Vertex' in clockwise sense
    /// @param x  First argument is the x coordinate of the input vertex
    /// @param y  Second argument is the y coordinate of the input vertex
    void add_vertex(double x, double y) {
        Vertex* v = new Vertex;
        v->x = x;
        v->y = y;
        v->leaving = NULL;
        vertices.push_back(v);
    }

    /// @brief This function adds face corresponding to halfedge e in a vector named 'Faces'
    /// @param e  'e' is the edge corresponding to which we are adding a face in the vector
    void add_face(Halfedge* e) {
        Face* f = new Face;
        f->edge = e;
        faces.push_back(f);
    }


    /// @brief This function adds a halfedge in a vector named 'Halfedge' for a particular source and destination vertex
    /// @param origin This is the vertex from where that particular halfedge is originating
    /// @param destination This is the vertex at which that particular halfedge is terminating
    /// @param incident_face Face corresponding to the origin to destination half edge
    void add_halfedge(Vertex* origin, Vertex* destination, Face* incident_face)
    {   //function to add different halfedges in halfedge vector using DCEL class
        Halfedge* e1 = new Halfedge;
        Halfedge* e2 = new Halfedge;
        e1->origin = origin;
        e1->twin = e2;
        e1->incident_face = incident_face;
        e1->next = NULL;
        e1->prev = NULL;
        e2->origin = destination;
        e2->twin = e1;
        e2->incident_face = incident_face;
        e2->next = NULL;
        e2->prev = NULL;
        origin->leaving = e1;
        halfedges.push_back(e1);
        halfedges.push_back(e2);
    }
};

/// @brief  This function takes input from user such as number and coordinates of vertices in a text file named 'input.txt'
/// @param no_of_vert Number of vertices in the input polygon
/// @param no_of_edge Number of edges in the input polygon
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
void input (int no_of_vert, int no_of_edge, DCEL &dcel)
{   //Take input from input.txt file and store vertices using DCEL class
    for (int i = 0; i < no_of_vert; i++)
    {
        double x, y;
        fp1 >> x >> y;
        dcel.add_vertex(x, y);
    }
    for (int i = 0; i < no_of_edge; i++)
    {
        dcel.add_halfedge(dcel.vertices[i], dcel.vertices[(i + 1) % no_of_edge], NULL);
    }
    return;
}

/// @brief This function prints the output edges generated upon decomposition of the polygon in a separate output.txt file
/// @param n N is the number of vertices in the input polygon provided by user
/// @param ans This vector contains the indexes of vertices that are to be joined for decomposing the polygon
void output(int n, vector<pair<int, int>> ans)
{
    // cout<<"called";
    // cout<<ans.size()<<endl;
    for (int i = 0; i < ans.size(); i++)
    {
        fp2 << ans[i].first << " " << ans[i].second << endl;
        fp2.flush();
    }
}

/// @brief This function is used for timing analysis of our code for different input sizes
/// @param time_taken This is the time taken by our code to execute for a particular input size
void time_req(double time_taken)
{
    fp3 << time_taken << endl;
    fp3.flush();
}

/// @brief This function connects each edge with its next edge and hence completes the polygon
/// @param n N is the number of vertices in the input polygon provided by user
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
void edgeConnect(int n, DCEL &dcel)
{   //edgeConnect function establishes connection between half edges of DCEL,which shapes the input polygon
    for (int i = 0; i < 2 * n; i += 2)
    {
        Halfedge* e1 = dcel.halfedges[i];
        Halfedge* e2 = dcel.halfedges[(i + 2) % (2 * n)];
        e1->next = e2;
        e2->prev = e1;
    }
    for (int i = 1; i < 2 * n; i += 2)
    {
        Halfedge* e1 = dcel.halfedges[i];
        Halfedge* e2 = dcel.halfedges[(i + 2) % (2 * n)];
        e2->next = e1;
        e1->prev = e2;
    }
}

/// @brief This function checks angle between vertices using cross product of coordinates of vertices
/// @param v1 The first vertex for computing angle
/// @param v2 The second vertex for computing angle
/// @param v3 The third vertex for computing angle
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
/// @return The function return 0 if the cross product is negative,means the angle is greater than 180 degrees else it returs 1 if angle is less than 180  degrees
int isAcuteAngle(int v1, int v2, int v3, DCEL dcel)
{   // find angle between 3 vertices using cross product,if cross product is negative,means that angle is greater than 180
    double crossproduct = (dcel.vertices[v1]->x - dcel.vertices[v2]->x) * (dcel.vertices[v3]->y - dcel.vertices[v2]->y) - (dcel.vertices[v1]->y - dcel.vertices[v2]->y) * (dcel.vertices[v3]->x - dcel.vertices[v2]->x);
    if (crossproduct < 0)
    {
        return 0;
    }
    return 1;
}

/// @brief This function finds if a particular vertex is in a vector of vertices
/// @param k K is the vectex to be searched in the vector
/// @param q Q is the vector containing vertices in which we are looking for k
void find(int k, vector<int> &q)
{   //if k is present in q then remove it,else return q
    vector<int> v5;
    for (int i = 0; i < q.size(); i++)
    {
        if (q[i] != k)
        {
            v5.push_back(q[i]);
        }
    }
    q = v5;

    return;

}

/// @brief This function finds smallest rectangle with sides parallel to axes which contains all vertices of the vector 'l'
/// @param dq dq is a vector which contains all vertices of vector 'l'
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
/// @return Returns a vector which contains smallest and largest x and y coordinate of the rectangle
vector<int> findRect(vector<int> dq, DCEL dcel)
{   //finding the smallest rectangle with sides parallel to axes containing all vertices of l
    vector<int> ans;
    int x1 = INT_MIN, x2 = INT_MAX;
    ans.push_back(x2);
    ans.push_back(x1);
    ans.push_back(x2);
    ans.push_back(x1);
    for (int i = 0; i < dq.size(); i++)
    {
        if (dcel.vertices[dq[i]]->x < ans[0])
            ans[0] = dcel.vertices[dq[i]]->x;

        if (dcel.vertices[dq[i]]->x > ans[1])
            ans[1] = dcel.vertices[dq[i]]->x;
        if (dcel.vertices[dq[i]]->y < ans[2])
            ans[2] = dcel.vertices[dq[i]]->y;
        if (dcel.vertices[dq[i]]->y > ans[3])
            ans[3] = dcel.vertices[dq[i]]->y;

    }
    return ans;   //ans vector contains coordinates of smallest rectangle
}

/// @brief This fucntion checks if a given vertex is inside the smallest rectangle created using function findRect
/// @param k k is the vertex for which we are checking if it is inside the rectangle or not
/// @param coord coord is the vector containing vertices of the rectanle in which are sarching for k
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
/// @return Returns true if the point is inside the rectangle else returns false
bool inside_rect (int k, vector<int> coord, DCEL dcel)
{   //checks if the point is inside the smallest rectangle obtained
    int v1 = dcel.vertices[k]->x;
    int v2 = dcel.vertices[k]->y;
    if (v1 <= coord[0] || v1 >= coord[1] || v2 <= coord[2] || v2 >= coord[3])
        return false;
    return true;
}


/// @brief This function checks if a given vertex is inside a polygon formed by vertices which are stored in a vector
/// @param k k is the vertex for which we are checking if it is inside the polygon or not
/// @param l l is a vector containing vertices of polygon in which we are looking for k
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
/// @return Returns true if the point is inside the polygon else returns false
bool inside_poly (int k, vector <int> l, DCEL dcel)
{   //if the vertex is inside polygon formed by vertices of vector l,return true else return false
    int sign, sign1;
    int v_x = dcel.vertices[l[k]]->x;
    int v_y = dcel.vertices[l[k]]->y;

    int v1_x = dcel.vertices[0]->x;
    int v1_y = dcel.vertices[0]->y;
    int v2_x = dcel.vertices[(1) % l.size()]->x;
    int v2_y = dcel.vertices[(1) % l.size()]->y;
    sign = ((v1_x - v2_x) * (v_y - v1_y)) - ((v1_y - v2_y) * (v_x - v1_x));
    for (int i = 1; i < l.size(); i++)
    {
        int v1_x = dcel.vertices[i]->x;
        int v1_y = dcel.vertices[i]->y;
        int v2_x = dcel.vertices[(i + 1) % l.size()]->x;
        int v2_y = dcel.vertices[(i + 1) % l.size()]->y;
        sign1 = ((v1_x - v2_x) * (v_y - v1_y)) - ((v1_y - v2_y) * (v_y - v1_y));
        if (sign * sign1 < 0)
            return false;
    }
    return true;
}

/// @brief This fucntion checks if there are any notches in a polygon by calculating the angle between its vertices
/// @param p P is the vector which contains vertices of the polygon for which we are checking notch function
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
/// @return Returns a vector of vertices which are notches
vector<int> notches_P (vector<int> p, DCEL dcel)
{   //checks for notches in polygon p using angle between vertices,if angle is greater than 180 means that there is a notch at that vertex
    vector < int> notch;
    for (int i = 0; i < p.size(); i++)
    {
        if  (isAcuteAngle(i, (i + 1) % p.size(), (i + 2) % p.size(), dcel) == 0)
            notch.push_back((i + 1) % p.size());
    }
    return notch;
}


void mergeConvexPolygons(int point1, int point2,DCEL dcel) {
    vector<int> merged;
    int x1 = dcel.vertices[point1]->x;
    int x2 = dcel.vertices[point2]->x;
    int y1 = dcel.vertices[point1]->y;
    int y2 = dcel.vertices[point2]->y;
    int i = 0; // Index for poly1
    int j = 0; // Index for poly2

    // Find the rightmost point in poly1 and the leftmost point in poly2
    int rightmost1 = 0;
    int leftmost2 = 0;
    for (int k = 1; k < min(x1,x2); k++) {
        if (dcel.vertices[k]->x > dcel.vertices[i]->x) {
            rightmost1 = k;
        }
    }
    for (int k = 1; k < min(y1,y2); k++) {
        if (dcel.vertices[k]->y > dcel.vertices[j]->y) {
            leftmost2 = k;
        }
    }

    // Merge the two convex polygons
    int p = rightmost1;
    int q = leftmost2;
    while (p < i || q < j) {
        merged.push_back(dcel.vertices[p]->x);
        int dx1 = dcel.vertices[(p + 1) % i]->x - dcel.vertices[p]->x;
        int dy1 = dcel.vertices[(p + 1) % j]->y - dcel.vertices[p]->y;
        int dx2 = dcel.vertices[(q + 1) % i]->x - dcel.vertices[q]->x;
        int dy2 = dcel.vertices[(q + 1) % j]->y - dcel.vertices[q]->y;

        // Choose the next point from poly1 or poly2
        if ((dx1 * dy2 - dy1 * dx2) < 0) {
            p = (p + 1) % i;
        } else {
            q = (q + 1) % j;
        }
    }

    return;
}

/// @brief This function finds notches in the polygon and splits the given polygon in various convex polygons using MP1 algorithm
/// @param no_of_vert N is the number of vertices in the input polygon provided by user
/// @param dcel Reference of DCEL class to access vertices and halfedges stored in DCEL class
void polygonDecomposition(int no_of_vert, DCEL &dcel)
{   //driver function to decompose the given polygon into multiple
    //convex polygons using MP1 algorithm given in research paper
    vector<int> p, l, lpvs;
    int v1, v2, v_i0, v_i, v_i1;
    int k;
    for (int i = 0; i < no_of_vert; i++)
    {
        p.push_back(i);
    }
    l.push_back(0);
    int z = 1;
    while (p.size() > 3)
    {
        v1 = l[l.size() - 1];
        for (int i = 0; i < p.size(); i++)
        {
            if (p[i] == v1)
            {
                v2 = p[(i + 1) % p.size()];
                break;
            }
        }
        l.clear();
        l.push_back(v1);
        l.push_back(v2);
        v_i0 = l[l.size() - 2];
        v_i = l[l.size() - 1];
        for (int i = 0; i < p.size(); i++)
        {
            if (p[i] == v_i)
            {
                v_i1 = p[(i + 1) % p.size()];
                break;
            }
        }

        while (isAcuteAngle(v_i0, v_i, v_i1, dcel) && isAcuteAngle(v_i, v_i1, v1, dcel) && isAcuteAngle(v_i1, v1, v2, dcel) && (l.size() < p.size()))
        {   //if all 3 angles formed from the vertices are acute then add more elements to l,
            //means that there is no notch in those vertices
            l.push_back(v_i1);
            v_i0 = v_i;
            v_i = v_i1;
            for (int i = 0; i < p.size(); i++)
            {
                if (p[i] == v_i1)
                {
                    v_i1 = p[(i + 1) % p.size()];
                    break;
                }
            }
        }

        if (l.size() != p.size())
        {
            lpvs = p;

            for (int i = 0; i < l.size(); i++)
            {
                find(l[i], lpvs);  //remove vertices from vector lpvs that are common with vector l
            }

            vector <int> notch = notches_P(p, dcel); //find notches in plygon P using notch function

            vector<int> v4;
            for (int i = 0; i < lpvs.size(); i++)
            {   //  from lpvs vector,remove all points that are not a notch
                int flag = 0;
                for (int j = 0; j < notch.size(); j++)
                {
                    if (lpvs[i] == notch[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag != 0)
                {
                    v4.push_back(lpvs[i]);
                }
            }
            lpvs = v4;

            while (lpvs.size() > 0)
            {
                vector<int> coord;
                coord = findRect(l, dcel); //Find smallest rectangle with sides parallel to axes which contains all points of vector l
                int x_small = coord[0];
                int x_large = coord[1];
                int y_small = coord[2];
                int y_large = coord[3];

                bool backward = false; int v = lpvs[0];
                vector<int > v7;

                while (!backward && lpvs.size() > 0)
                {
                    while (lpvs.size() != 0 && !inside_rect(v, coord, dcel))
                    {   //remove all points from lpvs that are not inside the rectangle formed
                        v7.clear();
                        v = lpvs[0];
                        if (!inside_rect(v, coord, dcel))
                        {
                            for (int i = 1; i < lpvs.size(); i++)
                            {
                                v7.push_back(lpvs[i]);
                            }
                            lpvs = v7;
                        }
                    }
                    if (lpvs.size() > 0)
                    {
                        if (inside_poly(v, l, dcel))
                        {
                            //if the point is inside the polygon formed using points of l,then find set of points in semiplace generated by
                            //lpvs[0],l[0] and l[last]
                            int sign = ((dcel.vertices[lpvs[0]]->x - dcel.vertices[l[0]]->x) * (dcel.vertices[l[l.size() - 1]]->y - dcel.vertices[lpvs[0]]->y)) - ((dcel.vertices[lpvs[0]]->y - dcel.vertices[l[0]]->y) * (dcel.vertices[l[l.size() - 1]]->x - dcel.vertices[lpvs[0]]->x));
                            int i;
                            vector<int> v8;
                            for (i = l.size() - 2; i >= 0; i--)
                            {
                                int sign1 = ((dcel.vertices[lpvs[0]]->x - dcel.vertices[l[0]]->x) * (dcel.vertices[l[i]]->y - dcel.vertices[lpvs[0]]->y)) - ((dcel.vertices[lpvs[0]]->y - dcel.vertices[l[0]]->y) * (dcel.vertices[l[i]]->x - dcel.vertices[lpvs[0]]->x));
                                if (sign1 * sign < 0)
                                {
                                    v8.push_back(l[i]);
                                }
                            }
                            if (v8[v8.size() - 1] != l[0])
                                v8.push_back(l[0]);
                            reverse(v8.begin(), v8.end());
                            l.clear();
                            l = v8;

                            k = i;
                            backward = true;
                            vector <int> v6;
                            for (int i = 0; i < lpvs.size() - 1; i++)
                            {
                                if (lpvs[i] != v)
                                {
                                    v6.push_back(i);

                                }
                            }
                            lpvs = v6;

                        }
                    }
                }
            }
        }

        if (l[l.size() - 1] != v2)
        {
            //3.5.1
            // if(l.size() != p.size())
            ans.push_back({l[0], l[l.size() - 1]}); //ans vector contains indexes of points that are to be joined
            //in order to decompose given polygon into convex polygons
            //3.5.2
            set<int> v0;
            set<int> temp;
            for (auto x : l)
                v0.insert(x);
            int a = l[0];
            int b = l[l.size() - 1];
            for (int i = 0; i < p.size(); i++) //Remove points from p that are common with vector l except the first and last
                //point of l as stated in the algorithm MP1
                if (v0.find(p[i]) == v0.end() || p[i] == a || p[i] == b)
                    temp.insert(p[i]);
            p.clear();
            for (auto x : temp)
                p.push_back(x);

        }
    }
}

/// @brief This is the driver function of the code which calls other functions like input,output and polygon decomposition
/// @return After the code is successfully executed,this function returns 0
int main()
{
    fp2.open("output.txt");
    fp1.open("input4.txt");
    fp3.open("time.txt");
    DCEL dcel;
    int no_of_vert, no_of_edge;
    fp1 >> no_of_vert;
    no_of_edge = no_of_vert;
    input(no_of_vert, no_of_edge, dcel); //Take input in vectex vector using DCEL class
    clock_t start, end;
    start = clock();
    edgeConnect(no_of_vert, dcel); //Make connections between halfedges of polygon and join each edge with its corresponding twin edge
    polygonDecomposition(no_of_vert, dcel);
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    time_req (time_taken);

    output(no_of_vert, ans); //Give output in output.txt file,which contains vex=rtices to be joined in order to decompose the polygon
    return 0;
}