//#ifdef GL_ES
//precision mediump float;
//#endif
//
//uniform sampler2D texture;
//uniform float time;
//uniform vec2 mouse;
//uniform vec2 resolution;
//
//float snoise(vec3 uv, float res)
//{
//	const vec3 s = vec3(1e0, 1e2, 1e3);
//	
//	uv *= res;
//	
//	vec3 uv0 = floor(mod(uv, res))*s;
//	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;
//	
//	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);
//
//	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
//		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);
//
//	vec4 r = fract(sin(v*1e-1)*1e3);
//	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
//	
//	r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
//	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
//	
//	return mix(r0, r1, f.z)*2.-1.;
//}
//
//
//void main( void ) {	
//	//vec2 p = gl_FragCoord.xy / resolution.xy;
//	//p.x = p.x - mouse.x / resolution.x;
//	//p.y = p.y + mouse.y / resolution.y;
//	//p.y = p.y - 1.0;
//	//
//	//p.x*=resolution.x/resolution.y;			
//	//		  	
//	//float color = 3.0 - (6.*length(p));		
//	//
//	//vec3 coord = vec3(atan(p.x,p.y)/6.2832, length(p)*0.4, .5);
//	//
//	//for(int i = 1; i <= 7; i++){
//	//	float power = pow(2.0, float(i));
//	//	color += (1.5 / power) * snoise(coord + vec3(0.,-time*.05, time*.01), power*16.);
//	//}
//	//
//	//gl_FragColor = vec4( color, pow(max(color,0.),2.)*0.4, pow(max(color,0.),3.)*0.15 , 1.0);
//	
//    // lookup the pixel in the texture
//    vec4 pixel  = texture2D(texture, vec2(gl_TexCoord[0].x,gl_TexCoord[0].y));
//    vec4 pixel2 = texture2D(texture, vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+.05));
//    
//    gl_FragColor = gl_Color * pixel+.5;
//    
//    // multiply it by the color
//    //if (pixel.r == 0.0) {
//    //    gl_FragColor = gl_Color * pixel;
//    //}
//    //else gl_FragColor = gl_Color * pixel*.2;
//}


uniform sampler2D tex;

//uniform float zoomScale;

//uniform float pixelW;
//uniform float pixelH;

uniform float texW[25];

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}



void main(void) {
    
    
    float num2  = .04;
    float alpha = 0;
    //if (texture2D(tex,vec2(gl_TexCoord[0].xy)).a != 0){
        alpha = 1;
        // horizontal blur
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[0] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[1] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[2] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[3] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[4] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[5] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[6] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[7] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[8] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[9] ,gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[10],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[11],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[12],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[13],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[14],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[15],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[16],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[17],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[18],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[19],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[20],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[21],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[22],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[23],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        if (texture2D(tex,vec2(gl_TexCoord[0].x+texW[24],gl_TexCoord[0].y)).a == 0) alpha -= num2;
        //0.001133 
        //0.002316 
        //0.00445	 
        //0.008033 
        //0.013627 
        //0.021724 
        //0.032542 
        //0.04581	 
        //0.0606	 
        //0.075333 
        //0.088001 
        //0.096603 
        //0.099654 
        //0.096603 
        //0.088001 
        //0.075333 
        //0.0606	 
        //0.04581	 
        //0.032542 
        //0.021724 
        //0.013627 
        //0.008033 
        //0.00445	 
        //0.002316 
        //0.001133 

        // vertical blur                                                                                           
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[0] )).a > 0) alpha += 0.002406*num2; else alpha -= 0.002406*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[1] )).a > 0) alpha += 0.009255*num2; else alpha -= 0.009255*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[2] )).a > 0) alpha += 0.027867*num2; else alpha -= 0.027867*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[3] )).a > 0) alpha += 0.065666*num2; else alpha -= 0.065666*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[4] )).a > 0) alpha += 0.121117*num2; else alpha -= 0.121117*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[5] )).a > 0) alpha += 0.174868*num2; else alpha -= 0.174868*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[6] )).a > 0) alpha += 0.197641*num2; else alpha -= 0.197641*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[7] )).a > 0) alpha += 0.174868*num2; else alpha -= 0.174868*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[8] )).a > 0) alpha += 0.121117*num2; else alpha -= 0.121117*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[9] )).a > 0) alpha += 0.065666*num2; else alpha -= 0.065666*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[10])).a > 0) alpha += 0.027867*num2; else alpha -= 0.027867*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[11])).a > 0) alpha += 0.009255*num2; else alpha -= 0.009255*num2;
        //if (texture2D(tex,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[12])).a > 0) alpha += 0.002406*num2; else alpha -= 0.002406*num2;
        
    //}
    if (alpha < 0) alpha = 0;
    if (alpha > 1) alpha = 1;
    //gl_FragColor = vec4(0,rand(gl_TexCoord[0].xy),0,alpha);
    gl_FragColor = vec4(0,0,0,alpha);
    
}




