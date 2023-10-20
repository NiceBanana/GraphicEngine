#include "ini_configuration.h"
#include "teken2D.h"
#include "teken3D.h"
#include "PlatBody.h"
#include "ZBuffer.h"
#include "Light.h"
#include "InfLight.h"

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
        image = teken2D::generateLines(linesToDraw, size, kleur_achtergrond,false);
    }
    else if(name=="Wireframe" or name=="ZBufferedWireframe" or name=="ZBuffering" or name=="LightedZBuffering"){
        Figures3D figs;
        Lights3D lichten;
        InfLights3D infLichten;
        bool isDiffuse=false;
        std::vector<double> eye= configuration["General"]["eye"].as_double_tuple_or_die();
        int nrFigures=configuration["General"]["nrFigures"].as_int_or_die();
        if(name=="LightedZBuffering") {
            int nrLights=configuration["General"]["nrLights"].as_int_or_die();
            for (int i = 0; i < nrLights;i++){
                auto sectieLight=configuration["Light"+std::to_string(i)];
                Light licht;
                auto ambient=sectieLight["ambientLight"].as_double_tuple_or_die();
                std::vector<double> diffuse;
                bool infinity=false;
                std::vector<double> direction;
                if(sectieLight["diffuseLight"].as_double_tuple_if_exists(diffuse)) {
                    isDiffuse=true;
                    infinity= sectieLight["infinity"].as_bool_or_die();
                    direction=sectieLight["direction"].as_double_tuple_or_die();
                    licht.diffuseLight=Color(diffuse[0],diffuse[1],diffuse[2]);
                }
                licht.ambientLight=Color(ambient[0],ambient[1],ambient[2]);
                if(infinity){
                    Vector3D Ld= Vector3D::vector(direction[0],direction[1],direction[2]);
                    InfLight lichtInf=InfLight(Ld);
                    lichtInf.ambientLight=licht.ambientLight;
                    lichtInf.diffuseLight=licht.diffuseLight;
                    infLichten.push_back(lichtInf);
                }
                else {
                    lichten.push_back(licht);
                }
            }
        }
        for(int i=0;i<nrFigures;i++){
            Figure figuur;
            auto sectieFig=configuration["Figure"+std::to_string(i)];
            std::string nameFig= sectieFig["type"].as_string_or_die();
            double scale=sectieFig["scale"].as_double_or_die();
            double rotateX=sectieFig["rotateX"].as_double_or_die();
            double rotateY=sectieFig["rotateY"].as_double_or_die();
            double rotateZ=sectieFig["rotateZ"].as_double_or_die();
            std::vector<double> center=sectieFig["center"].as_double_tuple_or_die();
            std::vector<double> ambientReflection;
            std::vector<double> diffuseReflection;
            if(name=="LightedZBuffering") {
                ambientReflection = sectieFig["ambientReflection"].as_double_tuple_or_die();
                if(isDiffuse){
                    diffuseReflection=sectieFig["diffuseReflection"].as_double_tuple_or_die();
                }
            }
            std::vector<double> linesColor;
            if(name!="LightedZBuffering"){
                linesColor=sectieFig["color"].as_double_tuple_or_die();
            }
             //oude kleur fixen/wegdoen
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
            else if(nameFig=="3DLSystem"){
                std::string fileName=sectieFig["inputfile"].as_string_or_die();
                LParser::LSystem3D l_system;
                std::ifstream input_stream(fileName);
                input_stream >> l_system;
                input_stream.close();
                std::string toDraw= teken3D::transformString(l_system);
                Color lijnkleur= Color(linesColor[0],linesColor[1],linesColor[2]);
                figuur=teken3D::stringToFigure(toDraw,l_system);
            }
            else if(nameFig=="Cube" or nameFig=="FractalCube"){
                figuur=PlatBody::generateCube();
            }
            else if(nameFig=="Tetrahedron" or nameFig=="FractalTetrahedron"){
                figuur=PlatBody::generateTetrahedron();
            }
            else if(nameFig=="Octahedron" or nameFig=="FractalOctahedron"){
                figuur=PlatBody::generateOctahedron();
            }
            else if(nameFig=="Icosahedron" or nameFig=="FractalIcosahedron"){
                figuur=PlatBody::generateIcosahedron();
            }
            else if(nameFig=="Dodecahedron" or nameFig=="FractalDodecahedron"){
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
            Color lijnkleur;
            if(name!="LightedZBuffering"){
                lijnkleur= Color(linesColor[0],linesColor[1],linesColor[2]);
                figuur.color=lijnkleur;
                figuur.ambientReflection=lijnkleur;
            }
            else{
                figuur.ambientReflection=Color(ambientReflection[0],ambientReflection[1],ambientReflection[2]);
                if(isDiffuse){
                    figuur.diffuseReflection=Color(diffuseReflection[0],diffuseReflection[1],diffuseReflection[2]);
                }
            }
            Matrix schaalMatrix=teken3D::matrixScale(scale);
            Matrix xMatrix=teken3D::matrixRotateX(rotateX);
            Matrix yMatrix=teken3D::matrixRotateY(rotateY);
            Matrix zMatrix=teken3D::matrixRotateZ(rotateZ);
            Vector3D centrum= Vector3D::point(center[0],center[1],center[2]);
            Matrix transMatrix=teken3D::matrixTranslate(centrum);
            Matrix figMatrix=schaalMatrix*xMatrix*yMatrix*zMatrix*transMatrix;
            teken3D::matrixTransform(figuur,figMatrix);
            int nr_iterations=0;
            double fractalScale;
            Figures3D fractal;
            if(nameFig=="FractalCube" or nameFig=="FractalTetrahedron" or nameFig=="FractalOctahedron" or nameFig=="FractalIcosahedron" or nameFig=="FractalDodecahedron") {
                 fractalScale= sectieFig["fractalScale"].as_double_or_die();
                nr_iterations = sectieFig["nrIterations"].as_int_or_die();
            }
            if(nr_iterations>0) {
                PlatBody::generateFractal(figuur,fractal,nr_iterations,fractalScale);
                figs.insert(figs.end(), fractal.begin(), fractal.end());
            }
            else {
                figs.push_back(figuur);
            }
        }
        Vector3D eyePoint=Vector3D::point(eye[0],eye[1],eye[2]);
        Matrix eyeMatrix=teken3D::matrixEye(eyePoint);
        teken3D::matrixTransformAll(figs,eyeMatrix);
        Lines2D linesToDraw=teken3D::projectionAll(figs);
        if(name=="Wireframe") {
            image = teken2D::generateLines(linesToDraw, size, kleur_achtergrond,false);
        }
        else if(name=="ZBufferedWireframe"){
            image=teken2D::generateLines(linesToDraw, size, kleur_achtergrond,true);
        }
        else if(name=="ZBuffering" or name=="LightedZBuffering"){
            double d;
            double dx;
            double dy;
            std::pair<unsigned int,unsigned int> imageSizes=teken2D::values4Triangle(linesToDraw,size,d,dx,dy);
            ZBuffer buffer(imageSizes.first,imageSizes.second);
            image=img::EasyImage(imageSizes.first,imageSizes.second);
            for(Figure fig:figs){
                fig.Figure::triangulate();
                for(auto vlak:fig.faces){
                    Vector3D A=fig.points[vlak.point_indexes[0]];
                    Vector3D B=fig.points[vlak.point_indexes[1]];
                    Vector3D C=fig.points[vlak.point_indexes[2]];
                    if(name=="LightedZBuffering"){
                        ZBuffer::draw_zbuf_triangle(buffer,image,A,B,C,d,dx,dy,fig.ambientReflection,fig.diffuseReflection,fig.specularReflection,fig.reflectionCoefficient,lichten,infLichten);
                    }
                    else {
                        InfLights3D emptyLichten={};
                        Lights3D lamp;
                        Light licht;
                        licht.ambientLight={1.0, 1.0, 1.0};
                        licht.diffuseLight={0.0, 0.0, 0.0},
                        licht.specularLight={0.0, 0.0, 0.0};
                        lamp.push_back(licht);
                        ZBuffer::draw_zbuf_triangle(buffer, image, A, B, C, d, dx, dy,fig.ambientReflection,fig.diffuseReflection,fig.specularReflection,fig.reflectionCoefficient,lamp,emptyLichten);
                    }
                }
            }
            //VBBESTANDEN DOWNLOADEN EN DAN TESTEN
        }
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
