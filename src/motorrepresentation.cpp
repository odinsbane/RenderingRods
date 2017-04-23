#include "display.hpp"

MotorRepresentation::MotorRepresentation(Motor* motor, glm::vec3 headAttachment, glm::vec3 tailAttachment, GLuint program){
    
    stalk = new RodRepresentation(motor->points, 0.1, program);
    stalk->red = 0;
    stalk->blue = 1;
    stalk->green = 0;

    headSpring = new SpringRepresentation(*(motor->points[0]), headAttachment);
    headSpring->red = 0;
    headSpring->blue = 1;
    headSpring->green = 0;
    
    tailSpring = new SpringRepresentation(*(motor->points[1]), tailAttachment);
    tailSpring->red = 0;
    tailSpring->blue = 1;
    tailSpring->green = 0;
}

void MotorRepresentation::updateData(
                 Motor* motor,
                 glm::vec3 headAttachment,
                                      glm::vec3 tailAttachment ){
    stalk->updateData(motor->points);
    headSpring->updateRepresentation(*(motor->points[0]), headAttachment);
    
    tailSpring->updateRepresentation(*(motor->points[1]), tailAttachment);
    
}
void MotorRepresentation::render(GLuint program){
    stalk->render(program);
    headSpring->render(program);
    tailSpring->render(program);
}
void MotorRepresentation::refreshBuffers(){
    stalk->refreshBuffers();
    headSpring->refreshBuffers();
    tailSpring->refreshBuffers();
}
