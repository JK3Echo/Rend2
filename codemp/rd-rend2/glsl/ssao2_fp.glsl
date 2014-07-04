#version 130

uniform sampler2D	u_DiffuseMap;
uniform sampler2D	u_ScreenDepthMap;
uniform sampler2D	u_NormalMap;

varying vec2		var_ScreenTex;
varying vec2		var_Dimensions;

vec3 readNormal(in vec2 coord)  
{  
     return normalize(texture2D(u_NormalMap, coord).xyz*2.0  - 1.0);  
}

vec3 posFromDepth(vec2 coord)
{
     float d = texture2D(u_ScreenDepthMap, coord).r;
     vec3 tray = mat3x3(gl_ProjectionMatrixInverse)*vec3((coord.x-0.5)*2.0,(coord.y-0.5)*2.0,1.0);
     return tray*d;
}

    //Ambient Occlusion form factor:
float aoFF(in vec3 ddiff,in vec3 cnorm, in float c1, in float c2)
{
          vec3 vv = normalize(ddiff);
          float rd = length(ddiff);
          return (1.0-clamp(dot(readNormal(var_ScreenTex+vec2(c1,c2)),-vv),0.0,1.0)) *
           clamp(dot( cnorm,vv ),0.0,1.0)* 
                 (1.0 - 1.0/sqrt(1.0/(rd*rd) + 1.0));
}
    //GI form factor:
float giFF(in vec3 ddiff,in vec3 cnorm, in float c1, in float c2)
{
          vec3 vv = normalize(ddiff);
          float rd = length(ddiff);
          return 1.0*clamp(dot(readNormal(var_ScreenTex+vec2(c1,c2)),-vv),0.0,1.0)*
                     clamp(dot( cnorm,vv ),0.0,1.0)/
                     (rd*rd+1.0);  
}

float Hash(in vec3 p)
{
    return fract(sin(dot(p,vec3(37.1,61.7, 12.4)))*3758.5453123);
}

float Noise(in vec3 p)
{
    vec3 i = floor(p);
	vec3 f = fract(p); 
	f *= f * (3.0-2.0*f);

    return mix(
		mix(mix(Hash(i + vec3(0.,0.,0.)), Hash(i + vec3(1.,0.,0.)),f.x),
			mix(Hash(i + vec3(0.,1.,0.)), Hash(i + vec3(1.,1.,0.)),f.x),
			f.y),
		mix(mix(Hash(i + vec3(0.,0.,1.)), Hash(i + vec3(1.,0.,1.)),f.x),
			mix(Hash(i + vec3(0.,1.,1.)), Hash(i + vec3(1.,1.,1.)),f.x),
			f.y),
		f.z);
}

void main()
{
    //read current normal,position and color.
    //vec3 n = readNormal(var_ScreenTex);
    vec3 p = posFromDepth(var_ScreenTex);
    vec3 col = texture2D(u_DiffuseMap, var_ScreenTex).rgb;
	vec3 n = col;

    //randomization texture
    //vec2 fres = vec2(var_Dimensions.x/128.0*5,var_Dimensions.y/128.0*5);
    //vec3 random = texture2D(grandom, var_ScreenTex*fres.xy).rgb;
	//vec3 random = texture2D(u_DiffuseMap, var_ScreenTex*fres.xy).rgb;
    //random = random*2.0-vec3(1.0);
	//vec3 random = vec3(Noise(p));
	vec3 random = vec3(1.0);
	//random = random*2.0-vec3(1.0);

    //initialize variables:
    float ao = 0.0;
    vec3 gi = vec3(0.0,0.0,0.0);
    float incx = 1.0/var_Dimensions.x;//*0.1;
    float incy = 1.0/var_Dimensions.y;//*0.1;
    float pw = incx;
    float ph = incy;
    float cdepth = texture2D(u_ScreenDepthMap, var_ScreenTex).r;

    //3 rounds of 8 samples each. 
    for(float i=0.0; i<3.0; ++i) 
    {
       float npw = (pw+0.0007*random.x)/cdepth;
       float nph = (ph+0.0007*random.y)/cdepth;

       vec3 ddiff = posFromDepth(var_ScreenTex+vec2(npw,nph))-p;
       vec3 ddiff2 = posFromDepth(var_ScreenTex+vec2(npw,-nph))-p;
       vec3 ddiff3 = posFromDepth(var_ScreenTex+vec2(-npw,nph))-p;
       vec3 ddiff4 = posFromDepth(var_ScreenTex+vec2(-npw,-nph))-p;
       vec3 ddiff5 = posFromDepth(var_ScreenTex+vec2(0,nph))-p;
       vec3 ddiff6 = posFromDepth(var_ScreenTex+vec2(0,-nph))-p;
       vec3 ddiff7 = posFromDepth(var_ScreenTex+vec2(npw,0))-p;
       vec3 ddiff8 = posFromDepth(var_ScreenTex+vec2(-npw,0))-p;

       ao+=  aoFF(ddiff,n,npw,nph);
       ao+=  aoFF(ddiff2,n,npw,-nph);
       ao+=  aoFF(ddiff3,n,-npw,nph);
       ao+=  aoFF(ddiff4,n,-npw,-nph);
       ao+=  aoFF(ddiff5,n,0,nph);
       ao+=  aoFF(ddiff6,n,0,-nph);
       ao+=  aoFF(ddiff7,n,npw,0);
       ao+=  aoFF(ddiff8,n,-npw,0);

       gi+=  giFF(ddiff,n,npw,nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(npw,nph)).rgb;
       gi+=  giFF(ddiff2,n,npw,-nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(npw,-nph)).rgb;
       gi+=  giFF(ddiff3,n,-npw,nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(-npw,nph)).rgb;
       gi+=  giFF(ddiff4,n,-npw,-nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(-npw,-nph)).rgb;
       gi+=  giFF(ddiff5,n,0,nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(0,nph)).rgb;
       gi+=  giFF(ddiff6,n,0,-nph)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(0,-nph)).rgb;
       gi+=  giFF(ddiff7,n,npw,0)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(npw,0)).rgb;
       gi+=  giFF(ddiff8,n,-npw,0)*texture2D(u_DiffuseMap, var_ScreenTex+vec2(-npw,0)).rgb;

       //increase sampling area:
       pw += incx;  
       ph += incy;    
    } 
    ao/=24.0;
    gi/=24.0;


    //gl_FragColor = vec4(col-vec3(ao)+gi*5.0,1.0);
	//gl_FragColor = vec4((col - vec3(ao) + gi * 5.0) * 1.2, 1.0);
	gl_FragColor = vec4((col + col + ((col - vec3(ao) + gi * 5.0))) * 0.33333333, 1.0);
}