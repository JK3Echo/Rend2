//// Nerevar's way to produce weather
//#include "cg_local.h" 
//
//#define RAINDISTANCE 500
//
//void CG_DrawRain(void) {
//	float rainDistance, rainNumber;
//	sfxHandle_t sfx;
//	if (cg_rain.integer <= 0)
//		return;
//	else if (cg_rain.integer > 2000)
//	trap->Cvar_Set("cg_rain", "10");		
//	rainDistance = cg_rainDistance.value < 10.0f ? 10.0f : cg_rainDistance.value;
//	rainNumber = cg_rain.value * (rainDistance / 1000.0f);
//	if (cg_rain.integer >= 1000) {
//		sfx = cgs.media.heavyRain;
//	}else if (cg_rain.integer >= 400) {
//		sfx = cgs.media.regularRain;
//	} else {
//		sfx = cgs.media.lightRain;
//	}
//	trap->S_UpdateEntityPosition(ENTITYNUM_NONE, cg.refdef.vieworg);
//	trap->S_AddLoopingSound(ENTITYNUM_NONE, cg.refdef.vieworg, vec3_origin, sfx, NULL);
//	if (cg.we.raintime <= cg.time) {
//		int i;						
//		for (i = 0; i < (int)rainNumber; i++) {
//			vec3_t start, fwd, end;
//			trace_t tr;
//				
//			float angle = cg.refdef.viewangles[YAW];
//			int seed = trap->Milliseconds() - i * 15;
//			float distance = Q_irand(0, 2.0f * rainDistance) - rainDistance;				
//				
//			AngleVectors(cg.refdef.viewangles, fwd, NULL, NULL);	 //<-Viewangles YAW		
//			VectorMA(cg.refdef.vieworg, rainDistance, fwd, start);
//			start[2] = cg.refdef.vieworg[2];
//				
//			start[0] = start[0] + (cos(angle)*distance);
//			start[1] = start[1] + (sin(angle)*distance);
//				
//			angle += DEG2RAD(90);
//			distance = Q_random(&seed) * 2.0f * rainDistance - rainDistance;
//				
//			start[0] = start[0] + (cos(angle)*distance);
//			start[1] = start[1] + (sin(angle)*distance);
//				
//			//GET SKY
//					
//			VectorCopy(start,end);
//			end[2] += 9999;				
//					
//			CG_Trace(&tr, start, NULL, NULL, end, 0, MASK_SOLID);
//				
//			if (tr.surfaceFlags & SURF_SKY) {
//				vec3_t angles;
//				if (tr.endpos[2] - start[2] > 200)
//					tr.endpos[2] = start[2] + 200;
//				tr.endpos[2] += Q_crandom(&seed) * 100.0f;
//				angles[PITCH] = 0;
//				angles[YAW] = 90;
//				angles[ROLL] = 0;
//				trap->FX_PlayEffectID(cgs.effects.rain, tr.endpos, angles, -1, 1, qfalse);
//			}
//		}			
//		cg.we.raintime = cg.time + 30;				
//	}
//}
//
//static qboolean CG_TraceToNotSky(trace_t *tr, const vec3_t start, const vec3_t end, const float size) {
//	int i;
//	/* scan 4 corners, if anyone of them isn't sky, then return and let's search with lower size */
//	for (i = 0; i < 4; i++) {
//		vec3_t offset, trStart, trEnd;
//		if (i == 0) VectorSet(offset, -size, -size, 0);
//		if (i == 1) VectorSet(offset, -size, size, 0);
//		if (i == 2) VectorSet(offset, size, -size, 0);
//		if (i == 3) VectorSet(offset, size, size, 0);
//		VectorSet(trStart, start[0] + offset[0], start[1] + offset[1], start[2]);
//		VectorSet(trEnd, end[0] + offset[0], end[1] + offset[1], end[2]);
//		CG_Trace(tr, trStart, NULL, NULL, trEnd, 0, MASK_SHOT);
//		if (!(tr->surfaceFlags & SURF_SKY)) {
//			return qtrue;
//		}
//	}
//	return qfalse;
//}
//////void CG_DrawSun(void) {
//////	int i;
//////	float size = 0.0f;
//////	trace_t tr;
//////	refEntity_t re;
//////	
//////	if (cg_sun.integer <= 0)
//////		return;
//////
//////	//CALCULATE SIZE //eats too much fps
//////	CG_Trace(&tr, cg.refdef.vieworg, NULL, NULL, cg.we.sunorigin, 0, MASK_SHOT);
//////	if (!(tr.surfaceFlags & SURF_SKY)) //not even worth it
//////		return;
//////	
//////	for (i = 31; i >= 0; i--) {
//////		qboolean notSky = CG_TraceToNotSky(&tr, cg.refdef.vieworg, cg.we.sunorigin, i);
//////		size = i;
//////		if (notSky)
//////			continue;
//////		if (tr.surfaceFlags & SURF_SKY) {
//////			float f, step = 1.0f / cg_sun.value;
//////			for (f = (float)(i + 1); f > (float)(i); f -= step) {
//////				notSky = CG_TraceToNotSky(&tr, cg.refdef.vieworg, cg.we.sunorigin, f);
//////				size = f;
//////				if (notSky)
//////					continue;
//////				if (tr.surfaceFlags & SURF_SKY) {
//////					break;
//////				}
//////			}
//////			break;
//////		}
//////	}
//////	size *= cg_sunSize.value;
//////	if (size <= 0.0f)
//////		return;
//////	memset(&re, 0, sizeof(refEntity_t));
//////	re.reType = RT_SPRITE;
//////	re.customShader = cgs.media.saberFlare;	
//////	VectorCopy(tr.endpos,re.origin);
//////		
//////	re.radius = size * Distance(re.origin, cg.refdef.vieworg) / 20;
//////	re.renderfx = RF_NODEPTH | RF_MINLIGHT;	
//////				
//////	re.shaderRGBA[0] = 255;
//////	re.shaderRGBA[1] = 230; //0.9
//////	re.shaderRGBA[2] = 191; //0.75
//////	re.shaderRGBA[3] = 255; 
//////		
//////	trap->R_AddRefEntityToScene( &re );
//////}
