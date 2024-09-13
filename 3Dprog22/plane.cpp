#include "plane.h"

Plane::Plane() : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    // Required Vertices
    //     v   x     y     z     r     g     b     u     v
    Vertex v0{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};    // Top-left vertex
    Vertex v1{ 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f};    // Top-right vertex
    Vertex v2{ 0.5, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};    // Bottom-right vertex
    Vertex v3{-0.5f,-0.5f,-0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};    // Bottom-left vertex

    mVertices.insert(mVertices.end(),
                     {
                     v0, v1, v2,    // Plane quad_1/2
                     v0, v2, v3,    // Plane quad_2/2
                     });

    mr = 0.1f;

    mMatrix.setToIdentity();
}

Plane::~Plane()
{

}

void Plane::init()
{
   initializeOpenGLFunctions();

   //********************** Making the object to be drawn **********************

   //Making and using the Vertex Array Object - VAO
   //VAO is a containter that holds VBOs
   glGenVertexArrays( 1, &mVAO );
   glBindVertexArray( mVAO );

   //Making and using the Vertex Buffer Object to hold vertices - VBO
   //Since the mVAO is bound, this VBO will belong to that VAO
   glGenBuffers( 1, &mVBO );
   glBindBuffer( GL_ARRAY_BUFFER, mVBO );

   //this sends the vertex data to the GPU:
   glBufferData( GL_ARRAY_BUFFER,                       //what buffer type
                 mVertices.size() * sizeof( Vertex ),   //how big buffer do we need
                 mVertices.data(),                      //the actual vertices
                 GL_STATIC_DRAW                         //should the buffer be updated on the GPU
                 );

   // 1st attribute buffer : vertices
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   glVertexAttribPointer(
               0,                           // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
               3,                           // size / number of elements of data type
               GL_FLOAT,                    // data type
               GL_FALSE,                    // normalize data
               sizeof(Vertex),         // stride
               reinterpret_cast<GLvoid*>(0  ));               // array buffer offset
   glEnableVertexAttribArray(0);


   // 2nd attribute buffer : colors
   // Same parameter list as above but attribute and offset is adjusted accoringly
   glVertexAttribPointer(
               1,
               3,
               GL_FLOAT,
               GL_FALSE,
               sizeof( Vertex ),
               reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6* sizeof(GLfloat)));
   glEnableVertexAttribArray(2);

   glBindVertexArray(0);
}

void Plane::draw(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

   //just to make the triangle rotate - tweak this: degree, x,   y,   z -axis
   if(mRotate)
       mMatrix.rotate(2.0f, 0.0f, 1.0f, 0.0f);
}

float Plane::getRadius()
{
    return mr;
}