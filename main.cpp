#include <QGuiApplication>
#include "window.h"


int main(int argc, char *argv[])
{



//    namedWindow("Output",1);

//        //initialize a 120X350 matrix of black pixels:
//        Mat output = Mat::zeros( 120, 350, CV_8UC3 );

//        //write text on the matrix:
//        putText(output,
//                "Hello World :)",
//                cvPoint(15,70),
//                FONT_HERSHEY_PLAIN,
//                3,
//                cvScalar(0,255,0),
//                4);

//        //display the image:
//        imshow("Output", output);

//        //wait for the user to press any key:
//        waitKey(0);
  QGuiApplication app(argc, argv);

  // Set OpenGL Version information
  // Note: This format must be set before show() is called.
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  //format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
  QSurfaceFormat::setDefaultFormat(format);
  format.setVersion(4,0);


#ifdef TESTE1
//  std::cout << "Teste da Macro: " << std::endl;
#endif

  // Set the window up
  Window window;
  window.setFormat(format);
  window.resize(QSize(600, 600));
  window.show();

  return app.exec();
}
