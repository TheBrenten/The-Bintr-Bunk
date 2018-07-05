#version 430
uniform bool arr[1920][1080];


void main(void) {
    float val = 0.5;
    //if (arr[int(gl_PointCoord[0].x)][int(gl_PointCoord[0].y)]) val = 1.0;
    //else val = 0.0;
    gl_FragColor = vec4(gl_FragCoord.x*.01,
                        gl_FragCoord.y*.05,1,1);
}