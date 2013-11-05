//TODO think of a good arch to distribute grid eval and draw functionalities
// taking into account blobs and implicit surfaces
//TODO La suma de implicit soft, ¿sin 1- ?
#include "grid.h"

using namespace std;

Grid::Grid(double _x, double _y, Implicit *_f)
{
    width = _x;
    height = _y;
    f = _f;


    drawMSCase[0] = &Grid::drawNothing;
    drawMSCase[1] = &Grid::drawEdgeLT;
    drawMSCase[2] = &Grid::drawEdgeTR;
    drawMSCase[3] = &Grid::drawEdgeH;
    drawMSCase[4] = &Grid::drawEdgeRB;
    drawMSCase[5] = &Grid::drawEdgeBRLT; //Ambiguous
    drawMSCase[6] = &Grid::drawEdgeV;
    drawMSCase[7] = &Grid::drawEdgeBL;
    drawMSCase[8] = &Grid::drawEdgeBL;
    drawMSCase[9] = &Grid::drawEdgeV;
    drawMSCase[10] = &Grid::drawEdgeBLTR; //Ambiguous
    drawMSCase[11] = &Grid::drawEdgeRB;
    drawMSCase[12] = &Grid::drawEdgeH;
    drawMSCase[13] = &Grid::drawEdgeTR;
    drawMSCase[14] = &Grid::drawEdgeLT;
    drawMSCase[15] = &Grid::drawNothing;




    //Fill the vector of grid points
    for(double i = 0; i <= width; i++){
        gridPoints.push_back(vector<Point2>());
        for(double j = 0; j <= height; j++){
            gridPoints.at(i).push_back(Point2(2*(i/width-0.5), 2*(j/height-0.5)));
        }
    }


    this->eval(f);

    this->findSurfacePoints(f);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            this->fillCell(i, j, f);
        }
    }


}

void Grid::draw(){

    glColor3f(0,0.5,1);

    /// drawing example
    glPushMatrix();

    //Draw the grid
    glColor3f(0.05,0.05,0.05);
    glLineWidth(1);
    for(int i = 0; i <= width; i++){
        glBegin(GL_LINE_STRIP);
        glVertex2f(2*(i/width-0.5),-1);
        glVertex2f(2*(i/width-0.5),1);
        glEnd();
    }
    for(int i = 0; i <= height; i++){
        glBegin(GL_LINE_STRIP);
        glVertex2f(-1,2*(i/height-0.5));
        glVertex2f(1,2*(i/height-0.5));
        glEnd();
    }

    for(double i = 0; i <= width; i++){
        for(double j = 0; j <= height; j++){

        }
    }


    //Draw the value of the function on each vertex
//    for(int i = 0; i <= width; i++){
//        for(int j = 0; j <= height; j++){
//            char c [16];
//            double val = values[(int)i][(int)j];
//           // cout << val << endl;
//            sprintf (c, "%.2f", val);
//            //TODO Why is the position messed up?
//            prog3d::ugl::drawText(c,gridPoints[i][j].getX()/2+0.5,gridPoints[i][j].getY()/2+0.5);
//           // cout << i/x-0.5 << endl;
//        }
//    }





    //Draw the colored points
//    glPointSize(7);
//    glBegin(GL_POINTS);
//    for(double i = 0; i <= width; i++){
//        for(double j = 0; j <= height; j++){
//            //cout << "values " << i << ", " << j << ": " << values[i][j] << endl;
//            if(values[i][j] > 0){
//                glColor3f(1.0, 0.0, 0.0);
//            } else {
//                glColor3f(0.0, 0.0, 1.0);
//            }
//            glVertex2f(gridPoints[i][j].getX(), gridPoints[i][j].getY());
//        }
//    }

//    glColor3f(1.0, 0.0, 1.0);
//    for(int i = 0; i < circlePoints.size(); i++){
//        glVertex2f(circlePoints.at(i).getX(), circlePoints.at(i).getY());
//    }

//    glEnd();

/*
    //Fill the surface
    for (double x = -1.0; x <= 1.0; x += .001){
          glBegin(GL_LINE_STRIP);
          for (double y=-1.0; y<=1.0; y+=.001){
              double value = f->eval(x,y);
              if(value >= 0){
                  glColor3f(value, 0.0, 1-value);
              } else {
                  glColor3f(1.0, 1.0, 1.0);
              }
              glVertex2f(x,y);
          }
          glEnd();
       }
*/

    glPopMatrix();

}

void Grid::eval(const Implicit *f){
    values.clear();
    values.resize((int)width+1);
    for(int i = 0; i <= width; i++){
        values[i].resize((int)height+1);
        for(int j = 0; j <= height; j++){
            double test = this->eval(f, gridPoints[i][j]);
            values[i][j] = test;
        }
    }        
}

double Grid::eval(const Implicit *f, Point2 &p){
    double x = p.getX();
    double y = p.getY();   
    double result = f->eval(x,y);
    return result;
}


void Grid::findSurfacePoints(const Implicit *f){

    for(int i = 0; i <= width; i++){
        for(int j = 0; j <= height; j++){
            //Horizontal edges
            if(i < width){
                double a = values[i][j];
                double b = values[i+1][j];
               // cout << "At "  << gridPoints[i][j].getX() << ", " << gridPoints[i][j].getY() << endl;
               // cout << "a = " << a << ", " << "b = " << b << endl;
                if(a/b < 0){
                    Point2 aux = this->interpolateAtZero(gridPoints[i][j], gridPoints[i+1][j], f);
                    circlePoints.push_back(aux);
                   // cout << "Point "  << aux.getX() << ", " << aux.getY() << endl;
                }
            }
            //Vertical edges
            if(j < height){
                double a = values[i][j];
                double b = values[i][j+1];
              //  cout << "At "  << gridPoints[i][j].getX() << ", " << gridPoints[i][j].getY() << endl;
              //  cout << "a = " << a << ", " << "b = " << b << endl;
                if(a/b < 0){
                    Point2 aux = this->interpolateAtZero(gridPoints[i][j], gridPoints[i][j+1], f);
                    circlePoints.push_back(aux);
                  //  cout << "Point " << aux.getX() << ", " << aux.getY() << endl;
                }
            }
        }
    }

}

//TODO div by 0
//TODO More robust. Order matters but shouldn't
Point2 Grid::interpolateAtZero(Point2 &a, Point2 &b, const Implicit *f){

    double v0 = this->eval(f, a);
    double v1 = this->eval(f, b);
    double dist = abs(v0-v1);
    double corr = abs(a.getX() - b.getX());
    double fab = corr/dist;
    double cx = a.getX() + abs(v0*fab);

    corr = abs(a.getY() - b.getY());
    fab = corr/dist;
    double cy = a.getY() + abs(v0*fab);

    return Point2(cx, cy);

}
//TODO Change the name of this function
void Grid::fillCell(int x, int y, const Implicit *f){
    //TODO array of function pointers? Problem at double lines
    double v0 = values[x][y];
    double v1 = values[x+1][y];
    double v2 = values[x+1][y+1];
    double v3 = values[x][y+1];

    Point2 p0 = gridPoints[x][y];
    Point2 p1 = gridPoints[x+1][y];
    Point2 p2 = gridPoints[x+1][y+1];
    Point2 p3 = gridPoints[x][y+1];

    //Indices of the function pointer array
    // an is 0 if vn is below 0. 1 otherwise
    int a0 = v0 < 0 ? 0 : 1;
    int a1 = v1 < 0 ? 0 : 1;
    int a2 = v2 < 0 ? 0 : 1;
    int a3 = v3 < 0 ? 0 : 1;

    int index = a0*8 + a1*4 + a2*2 + a3;

    (this->*drawMSCase[index])(p0, p1, p2, p3, f) ;
    //cout << "Cell " << x << ", " << y << endl;
    //cout << "Drawing Case " << index << endl;

}

void Grid::drawEdgeBL(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p0, p1, f);
    Point2 b = this->interpolateAtZero(p0, p3, f);
    this->drawSurfaceLine(a, b, f);

}

void Grid::drawEdgeH(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p0, p3, f);
    Point2 b = this->interpolateAtZero(p1, p2, f);
    this->drawSurfaceLine(a, b, f);
}

void Grid::drawEdgeLT(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p0, p3, f);
    Point2 b = this->interpolateAtZero(p3, p2, f);
    this->drawSurfaceLine(a, b, f);
}

void Grid::drawEdgeRB(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p0, p1, f);
    Point2 b = this->interpolateAtZero(p1, p2, f);
    this->drawSurfaceLine(a, b, f);
}

void Grid::drawEdgeTR(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p1, p2, f);
    Point2 b = this->interpolateAtZero(p3, p2, f);
    this->drawSurfaceLine(a, b, f);
}

void Grid::drawEdgeV(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    Point2 a = this->interpolateAtZero(p3, p2, f);
    Point2 b = this->interpolateAtZero(p0, p1, f);
    this->drawSurfaceLine(a, b, f);
}

void Grid::drawEdgeBLTR(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    cout << "BLTR" << endl;
    this->drawEdgeBL(p0, p1, p2, p3, f);
    this->drawEdgeTR(p0, p1, p2, p3, f);
}

void Grid::drawEdgeBRLT(Point2 p0, Point2 p1, Point2 p2, Point2 p3, const Implicit *f){
    cout << "BRTL" << endl;
    this->drawEdgeRB(p0, p1, p2, p3, f);
    this->drawEdgeLT(p0, p1, p2, p3, f);
}

//TODO change name to draw line if possible
void Grid::drawSurfaceLine(Point2 a, Point2 b, const Implicit *f){
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(a.getX(), a.getY());
    glVertex2f(b.getX(), b.getY());
    glEnd();
}

void Grid::drawNothing(Point2, Point2, Point2, Point2, const Implicit *f){
    //Dummy for void marching squares cases
}
