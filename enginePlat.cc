#include "ini_configuration.h"
#include "teken2D.h"
#include "teken3D.h"
#include "PlatBody.h"

#include <fstream>

using Lines2D=std::list<Line2D>;

img::EasyImage generate_image(const ini::Configuration &configuration)
{
    img::EasyImage image;
    std::string name= configuration["General"]["type"].as_string_or_die();
    int size = configuration["General"]["size"].as_int_or_die();
    std::vector<double> kleur_achtergrond = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    if(name=="2DLSystem") {
        std::string L2Dfile = configuration["2DLSystem"]["inputfile"].as_string_or_die();
        std::vector<double> kleur_figuur = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
        LParser::LSystem2D l_system;
        std::ifstream input_stream(L2Dfile);
        input_stream >> l_system;
        input_stream.close();
        Color lijnkleur = Color(kleur_figuur[0], kleur_figuur[1], kleur_figuur[2]);
        Lines2D linesToDraw = teken2D::stringToLines(teken2D::transformString(l_system), l_system, lijnkleur); //
        image = teken2D::generateLines(linesToDraw, size, kleur_achtergrond);
    }
    else if(name=="Wireframe"){
        Figures3D figs;
        std::vector<double> eye= configuration["General"]["eye"].as_double_tuple_or_die();
        int nrFigures=configuration["General"]["nrFigures"].as_int_or_die();
        for(int i=0;i<nrFigures;i++){
            Figure figuur;
            auto sectieFig=configuration["Figure"+std::to_string(i)];
            std::string nameFig= sectieFig["type"].as_string_or_die();
            double scale=sectieFig["scale"].as_double_or_die();
            double rotateX=sectieFig["rotateX"].as_double_or_die();
            double rotateY=sectieFig["rotateY"].as_double_or_die();
            double rotateZ=sectieFig["rotateZ"].as_double_or_die();
            std::vector<double> center=sectieFig["center"].as_double_tuple_or_die();
            std::vector<double> linesColor=sectieFig["color"].as_double_tuple_or_die();
            if(nameFig=="LineDrawing"){
                int nrPoints=sectieFig["nrPoints"].as_int_or_die();
                for(int j=0;j<nrPoints;j++){
                    std::vector<double> point;
                    point=sectieFig["point"+std::to_string(j)].as_double_tuple_or_die();
                    Vector3D punt= Vector3D::point(point[0],point[1],point[2]);
                    figuur.points.push_back(punt);
                }
                int nrLines=sectieFig["nrLines"].as_int_or_die();
                for(int n=0;n<nrLines;n++){
                    std::vector<int> line;
                    line=sectieFig["line"+std::to_string(n)].as_int_tuple_or_die();
                    Face lijn;
                    lijn.point_indexes=line;
                    figuur.faces.push_back(lijn);
                }
            }
            else if(nameFig=="Cube"){
                figuur=PlatBody::generateCube();
            }
            else if(nameFig=="Tetrahedron"){
                figuur=PlatBody::generateTetrahedron();
            }
            else if(nameFig=="Octahedron"){
                figuur=PlatBody::generateOctahedron();
            }
            else if(nameFig=="Icosahedron"){
                figuur=PlatBody::generateIcosahedron();
            }
            else if(nameFig=="Dodecahedron"){
                figuur=PlatBody::generateDodecahedron();
            }
            else if(nameFig=="Cylinder" or nameFig=="Cone"){
                int n=sectieFig["n"].as_int_or_die();
                double h=sectieFig["height"].as_double_or_die();
                if(nameFig=="Cylinder") {
                    figuur = PlatBody::generateCylinder(n, h);
                }
                else{
                    figuur=PlatBody::generateCone(n,h);
                }
            }
            else if(nameFig=="Sphere"){
                int n=sectieFig["n"].as_int_or_die();
                figuur=PlatBody::generateSphere(1,n);
            }
            else if(nameFig=="Torus"){
                double R=sectieFig["R"].as_double_or_die();
                double r=sectieFig["r"].as_double_or_die();
                int n=sectieFig["n"].as_int_or_die();
                int m=sectieFig["m"].as_int_or_die();
                figuur=PlatBody::generateTorus(R,r,n,m);
            }
            Color lijnkleur= Color(linesColor[0],linesColor[1],linesColor[2]);
            figuur.color=lijnkleur;
            Matrix schaalMatrix=teken3D::matrixScale(scale);
            Matrix xMatrix=teken3D::matrixRotateX(rotateX);
            Matrix yMatrix=teken3D::matrixRotateY(rotateY);
            Matrix zMatrix=teken3D::matrixRotateZ(rotateZ);
            Vector3D centrum= Vector3D::point(center[0],center[1],center[2]);
            Matrix transMatrix=teken3D::matrixTranslate(centrum);
            Matrix figMatrix=schaalMatrix*xMatrix*yMatrix*zMatrix*transMatrix;
            teken3D::matrixTransform(figuur,figMatrix);
            figs.push_back(figuur);
        }
        Vector3D eyePoint=Vector3D::point(eye[0],eye[1],eye[2]);
        Matrix eyeMatrix=teken3D::matrixEye(eyePoint);
        teken3D::matrixTransformAll(figs,eyeMatrix);
        Lines2D linesToDraw=teken3D::projectionAll(figs);
        image = teken2D::generateLines(linesToDraw, size, kleur_achtergrond);
    }
    return image;
}

int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        std::ifstream fileIn("filelist");
                        std::string filelistName;
                        while (std::getline(fileIn, filelistName)) {
                                args.push_back(filelistName);
                        }
                }
                for(std::string fileName : args)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(fileName);
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << fileName << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
