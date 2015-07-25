attribute vec3	attr_Position;
attribute vec4	attr_TexCoord0;

uniform mat4	u_ModelMatrix;
uniform mat4	u_ModelViewProjectionMatrix;

uniform vec4	u_ViewInfo; // zfar / znear, zfar
uniform vec2	u_Dimensions;
uniform vec4	u_Local0; // lightOrg
uniform vec4	u_Local1; // lightColor
uniform vec4	u_Local2; // lightScreenPos (x,y), testvar
uniform vec4	u_Local3; // exposure, decay, density, weight

varying vec2	var_TexCoords;
varying vec4	var_ViewInfo; // zfar / znear, zfar
varying vec2	var_Dimensions;
varying vec4	var_Local0; // lightOrg
varying vec4	var_Local1; // lightColor
varying vec4	var_Local2; // lightScreenPos (x,y), testvar
varying vec4	var_Local3; // exposure, decay, density, weight
varying vec2	var_LightScreenPos;
varying vec4	var_LightOrg;

void main()
{
	gl_Position = u_ModelViewProjectionMatrix * vec4(attr_Position, 1.0);
	//var_Local0 = u_ModelViewProjectionMatrix * vec4(u_Local0.xyz, 1.0);
	var_TexCoords = attr_TexCoord0.st;
	var_ViewInfo = u_ViewInfo;
	var_Dimensions = u_Dimensions.st;
	var_Local0.a = u_Local0.a;
	var_Local1 = u_Local1.rgba;
	var_Local2 = u_Local2.rgba;
	var_Local3 = u_Local3.rgba;
	
	//vec4 transformedPos = var_Local0; // usually just the typical multiplications with modelview and projection matrices.
	//vec4 NDC = transformedPos / transformedPos.w;
	//vec2 normalizedScreenPosAsYouDefined = NDC.xy * 0.5 + 0.5; // 0:0 is bottom left, flip y (ie. 1-y) if you want it to be top-left, which is quite freakish - but you might prefer it coming from DX.
	//var_LightScreenPos = normalizedScreenPosAsYouDefined;
	var_LightScreenPos = u_Local2.xy;
	//var_LightOrg = NDC;
}