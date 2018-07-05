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


uniform sampler2D originalTexture;
uniform sampler2D horizontalPass;

//uniform float zoomScale;

//uniform float pixelW;
//uniform float pixelH;

uniform float texH[25];

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}



void main(void) {
    
    
    float num2  = .04;
    float alpha = 0;
    //vec4 color = vec4(0,0,rand(gl_TexCoord[0].xy),0);
    vec4 color = vec4(0,0,0,0);
    
    //if (texture2D(originalTexture,vec2(gl_TexCoord[0].xy)).a != 0){
        // vertical blur                                                                                           
        //float test = 1;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[0] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[1] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[2] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[3] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[4] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[5] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[6] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[7] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[8] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[9] )).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[10])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[11])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[12])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[13])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[14])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[15])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[16])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[17])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[18])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[19])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[20])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[21])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[22])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[23])).a == 0) test -= num2;
        //if (texture2D(originalTexture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[24])).a == 0) test -= num2;
        //test = test/25;
        //test -= .5;
        //test *= 2;
        //if (alpha < 0) alpha = 0;
        //if (alpha > 1) alpha = 1;
        
        float extra = 0;
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[0] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[1] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[2] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[3] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[4] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[5] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[6] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[7] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[8] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[9] )).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[10])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[11])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[12])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[13])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[14])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[15])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[16])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[17])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[18])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[19])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[20])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[21])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[22])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[23])).a);
        extra+=(texture2D(horizontalPass,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+texH[24])).a);
        extra = extra/25;
        extra -= .5;
        extra *= 2;
        //alpha = test;
        alpha = extra;
        //alpha = extra + test;
        //extra -= 12.5f;
        //extra /= 25.f;
        ////extra*=2.f;
        //alpha += extra;
        if (alpha < 0) alpha = 0;
        if (alpha > 1) alpha = 1;
        
        color.a = alpha;
        
    //}
    gl_FragColor = color;
    //gl_FragColor = vec4(0,0,0,extra);
    
}




