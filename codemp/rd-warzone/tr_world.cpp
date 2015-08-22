/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include "tr_local.h"



/*
================
R_CullSurface

Tries to cull surfaces before they are lighted or
added to the sorting list.
================
*/
static qboolean	R_CullSurface( msurface_t *surf, int entityNum ) {
#if 0
	if ( r_nocull->integer || surf->cullinfo.type == CULLINFO_NONE) {
		return qfalse;
	}

	if ( *surf->data == SF_GRID && r_nocurves->integer ) {
		return qtrue;
	}

	if ( *surf->data != SF_FACE && *surf->data != SF_TRIANGLES && *surf->data != SF_POLY && *surf->data != SF_VBO_MESH && *surf->data != SF_GRID )
	{
		return qtrue;
	}

	// plane cull
	if ( *surf->data == SF_FACE && r_facePlaneCull->integer )
	{
		float d = DotProduct( tr.ori.viewOrigin, surf->cullinfo.plane.normal ) - surf->cullinfo.plane.dist;

		// shadowmaps draw back surfaces
		if ( tr.viewParms.flags & (VPF_SHADOWMAP | VPF_DEPTHSHADOW) )
		{
			if (surf->shader->cullType == CT_FRONT_SIDED)
			{
				surf->shader->cullType = CT_BACK_SIDED;
			}
			else
			{
				surf->shader->cullType = CT_FRONT_SIDED;
			}
		}

		// do proper cull for orthographic projection
		if (tr.viewParms.flags & VPF_ORTHOGRAPHIC) {
			d = DotProduct(tr.viewParms.ori.axis[0], surf->cullinfo.plane.normal);
			if ( surf->shader->cullType == CT_FRONT_SIDED ) {
				if (d > 0)
					return qtrue;
			} else {
				if (d < 0)
					return qtrue;
			}
			return qfalse;
		}

		// don't cull exactly on the plane, because there are levels of rounding
		// through the BSP, ICD, and hardware that may cause pixel gaps if an
		// epsilon isn't allowed here
		if ( surf->shader->cullType == CT_FRONT_SIDED )
		{
			if ( d < -8.0f )
			{
				return qtrue;
			}
		}
		else if ( surf->shader->cullType == CT_BACK_SIDED )
		{
			if ( d > 8.0f )
			{
				return qtrue;
			}
		}
		else if (surf->shader->cullType == CT_TWO_SIDED)
		{
			return qfalse;
		}
		else
		{
			return qfalse;
		}
	}

	if (surf->cullinfo.type & CULLINFO_SPHERE)
	{
		int 	sphereCull;

		if ( entityNum != REFENTITYNUM_WORLD ) {
			sphereCull = R_CullLocalPointAndRadius( surf->cullinfo.localOrigin, surf->cullinfo.radius );
		} else {
			sphereCull = R_CullPointAndRadius( surf->cullinfo.localOrigin, surf->cullinfo.radius );
		}

		if ( sphereCull == CULL_OUT )
		{
			return qtrue;
		}
	}

	if (surf->cullinfo.type & CULLINFO_BOX)
	{
		int boxCull;

		if ( entityNum != REFENTITYNUM_WORLD ) {
			boxCull = R_CullLocalBox( surf->cullinfo.bounds );
		} else {
			boxCull = R_CullBox( surf->cullinfo.bounds );
		}

		if ( boxCull == CULL_OUT )
		{
			return qtrue;
		}
	}

	return qfalse;
#else
	if ( r_nocull->integer || surf->cullinfo.type == CULLINFO_NONE) {
		return qfalse;
	}

	if ( *surf->data == SF_GRID && r_nocurves->integer ) {
		return qtrue;
	}

	if (surf->cullinfo.type & CULLINFO_PLANE)
	{
		// Only true for SF_FACE, so treat like its own function
		float			d;
		cullType_t ct;

		if ( !r_facePlaneCull->integer ) {
			return qfalse;
		}

		ct = surf->shader->cullType;

		if (ct == CT_TWO_SIDED)
		{
			return qfalse;
		}

		// don't cull for depth shadow
		/*
		if ( tr.viewParms.flags & VPF_DEPTHSHADOW )
		{
			return qfalse;
		}
		*/

		// shadowmaps draw back surfaces
		if ( tr.viewParms.flags & (VPF_SHADOWMAP | VPF_DEPTHSHADOW) )
		{
			if (ct == CT_FRONT_SIDED)
			{
				ct = CT_BACK_SIDED;
			}
			else
			{
				ct = CT_FRONT_SIDED;
			}
		}

		// do proper cull for orthographic projection
		if (tr.viewParms.flags & VPF_ORTHOGRAPHIC) {
			d = DotProduct(tr.viewParms.ori.axis[0], surf->cullinfo.plane.normal);
			if ( ct == CT_FRONT_SIDED ) {
				if (d > 0)
					return qtrue;
			} else {
				if (d < 0)
					return qtrue;
			}
			return qfalse;
		}

		d = DotProduct (tr.ori.viewOrigin, surf->cullinfo.plane.normal);

		// don't cull exactly on the plane, because there are levels of rounding
		// through the BSP, ICD, and hardware that may cause pixel gaps if an
		// epsilon isn't allowed here 
		if ( ct == CT_FRONT_SIDED ) {
			if ( d < surf->cullinfo.plane.dist - 8 ) {
				return qtrue;
			}
		} else {
			if ( d > surf->cullinfo.plane.dist + 8 ) {
				return qtrue;
			}
		}

		return qfalse;
	}

	if (surf->cullinfo.type & CULLINFO_SPHERE)
	{
		int 	sphereCull;

		if ( entityNum != REFENTITYNUM_WORLD ) {
			sphereCull = R_CullLocalPointAndRadius( surf->cullinfo.localOrigin, surf->cullinfo.radius );
		} else {
			sphereCull = R_CullPointAndRadius( surf->cullinfo.localOrigin, surf->cullinfo.radius );
		}

		if ( sphereCull == CULL_OUT )
		{
			return qtrue;
		}
	}

	if (surf->cullinfo.type & CULLINFO_BOX)
	{
		int boxCull;

		if ( entityNum != REFENTITYNUM_WORLD ) {
			boxCull = R_CullLocalBox( surf->cullinfo.bounds );
		} else {
			boxCull = R_CullBox( surf->cullinfo.bounds );
		}

		if ( boxCull == CULL_OUT )
		{
			return qtrue;
		}
	}

	return qfalse;
#endif
}


/*
====================
R_DlightSurface

The given surface is going to be drawn, and it touches a leaf
that is touched by one or more dlights, so try to throw out
more dlights if possible.
====================
*/
#if 0
static int R_DlightSurface( msurface_t *surf, int dlightBits ) {
	float       d;
	int         i;
	dlight_t    *dl;
	
	if ( surf->cullinfo.type & CULLINFO_PLANE )
	{
		for ( i = 0 ; i < tr.refdef.num_dlights ; i++ ) {
			if ( ! ( dlightBits & ( 1 << i ) ) ) {
				continue;
			}
			dl = &tr.refdef.dlights[i];
			d = DotProduct( dl->origin, surf->cullinfo.plane.normal ) - surf->cullinfo.plane.dist;
			if ( d < -dl->radius || d > dl->radius ) {
				// dlight doesn't reach the plane
				dlightBits &= ~( 1 << i );
			}
		}
	}
	
	if ( surf->cullinfo.type & CULLINFO_BOX )
	{
		for ( i = 0 ; i < tr.refdef.num_dlights ; i++ ) {
			if ( ! ( dlightBits & ( 1 << i ) ) ) {
				continue;
			}
			dl = &tr.refdef.dlights[i];
			if ( dl->origin[0] - dl->radius > surf->cullinfo.bounds[1][0]
				|| dl->origin[0] + dl->radius < surf->cullinfo.bounds[0][0]
				|| dl->origin[1] - dl->radius > surf->cullinfo.bounds[1][1]
				|| dl->origin[1] + dl->radius < surf->cullinfo.bounds[0][1]
				|| dl->origin[2] - dl->radius > surf->cullinfo.bounds[1][2]
				|| dl->origin[2] + dl->radius < surf->cullinfo.bounds[0][2] ) {
				// dlight doesn't reach the bounds
				dlightBits &= ~( 1 << i );
			}
		}
	}

	if ( surf->cullinfo.type & CULLINFO_SPHERE )
	{
		for ( i = 0 ; i < tr.refdef.num_dlights ; i++ ) {
			if ( ! ( dlightBits & ( 1 << i ) ) ) {
				continue;
			}
			dl = &tr.refdef.dlights[i];
			if (!SpheresIntersect(dl->origin, dl->radius, surf->cullinfo.localOrigin, surf->cullinfo.radius))
			{
				// dlight doesn't reach the bounds
				dlightBits &= ~( 1 << i );
			}
		}
	}

	switch(*surf->data)
	{
		case SF_FACE:
		case SF_GRID:
		case SF_TRIANGLES:
		case SF_VBO_MESH:
			((srfBspSurface_t *)surf->data)->dlightBits = dlightBits;
			break;

		default:
			dlightBits = 0;
			break;
	}

	if ( dlightBits ) {
		tr.pc.c_dlightSurfaces++;
	} else {
		tr.pc.c_dlightSurfacesCulled++;
	}

	return dlightBits;
}
#endif

/*
====================
R_PshadowSurface

Just like R_DlightSurface, cull any we can
====================
*/
static int R_PshadowSurface( msurface_t *surf, int pshadowBits ) {
#if 0
	float       d;
	int         i;
	pshadow_t    *ps;
	
	if ( surf->cullinfo.type & CULLINFO_PLANE )
	{
		for ( i = 0 ; i < tr.refdef.num_pshadows ; i++ ) {
			if ( ! ( pshadowBits & ( 1 << i ) ) ) {
				continue;
			}
			ps = &tr.refdef.pshadows[i];
			d = DotProduct( ps->lightOrigin, surf->cullinfo.plane.normal ) - surf->cullinfo.plane.dist;
			if ( d < -ps->lightRadius || d > ps->lightRadius ) {
				// pshadow doesn't reach the plane
				pshadowBits &= ~( 1 << i );
			}
		}
	}
	
	if ( surf->cullinfo.type & CULLINFO_BOX )
	{
		for ( i = 0 ; i < tr.refdef.num_pshadows ; i++ ) {
			if ( ! ( pshadowBits & ( 1 << i ) ) ) {
				continue;
			}
			ps = &tr.refdef.pshadows[i];
			if ( ps->lightOrigin[0] - ps->lightRadius > surf->cullinfo.bounds[1][0]
				|| ps->lightOrigin[0] + ps->lightRadius < surf->cullinfo.bounds[0][0]
				|| ps->lightOrigin[1] - ps->lightRadius > surf->cullinfo.bounds[1][1]
				|| ps->lightOrigin[1] + ps->lightRadius < surf->cullinfo.bounds[0][1]
				|| ps->lightOrigin[2] - ps->lightRadius > surf->cullinfo.bounds[1][2]
				|| ps->lightOrigin[2] + ps->lightRadius < surf->cullinfo.bounds[0][2] 
				|| BoxOnPlaneSide(surf->cullinfo.bounds[0], surf->cullinfo.bounds[1], &ps->cullPlane) == 2 ) {
				// pshadow doesn't reach the bounds
				pshadowBits &= ~( 1 << i );
			}
		}
	}

	if ( surf->cullinfo.type & CULLINFO_SPHERE )
	{
		for ( i = 0 ; i < tr.refdef.num_pshadows ; i++ ) {
			if ( ! ( pshadowBits & ( 1 << i ) ) ) {
				continue;
			}
			ps = &tr.refdef.pshadows[i];
			if (!SpheresIntersect(ps->viewOrigin, ps->viewRadius, surf->cullinfo.localOrigin, surf->cullinfo.radius)
				|| DotProduct( surf->cullinfo.localOrigin, ps->cullPlane.normal ) - ps->cullPlane.dist < -surf->cullinfo.radius)
			{
				// pshadow doesn't reach the bounds
				pshadowBits &= ~( 1 << i );
			}
		}
	}

	switch(*surf->data)
	{
		case SF_FACE:
		case SF_GRID:
		case SF_TRIANGLES:
		case SF_VBO_MESH:
			((srfBspSurface_t *)surf->data)->pshadowBits = pshadowBits;
			break;

		default:
			pshadowBits = 0;
			break;
	}

	if ( pshadowBits ) {
		//tr.pc.c_dlightSurfaces++;
	}

	return pshadowBits;
#else
	return 0;
#endif
}


/*
======================
R_AddWorldSurface
======================
*/
static void R_AddWorldSurface( msurface_t *surf, int entityNum, int dlightBits, int pshadowBits ) {
	// FIXME: bmodel fog?

	// try to cull before dlighting or adding
	if ( R_CullSurface( surf, entityNum ) ) {
		return;
	}

	// check for dlighting
	/*if ( dlightBits ) {
		dlightBits = R_DlightSurface( surf, dlightBits );
		dlightBits = ( dlightBits != 0 );
	}*/

	// check for pshadows
	/*if ( pshadowBits ) *//*{
		pshadowBits = R_PshadowSurface( surf, pshadowBits);
		pshadowBits = ( pshadowBits != 0 );
	}*/

	R_AddDrawSurf( surf->data, entityNum, surf->shader, surf->fogIndex, dlightBits, R_IsPostRenderEntity (entityNum, tr.currentEntity), surf->cubemapIndex );
}

/*
=============================================================

	BRUSH MODELS

=============================================================
*/

/*
=================
R_AddBrushModelSurfaces
=================
*/
void R_AddBrushModelSurfaces ( trRefEntity_t *ent, int entityNum ) {
	bmodel_t	*bmodel;
	int			clip;
	model_t		*pModel;
	int			i;

	pModel = R_GetModelByHandle( ent->e.hModel );

	bmodel = pModel->data.bmodel;

	clip = R_CullLocalBox( bmodel->bounds );
	if ( clip == CULL_OUT ) {
		return;
	}
	
	R_SetupEntityLighting( &tr.refdef, ent );
	R_DlightBmodel( bmodel );

	for ( i = 0 ; i < bmodel->numSurfaces ; i++ ) {
		int surf = bmodel->firstSurface + i;

		if (tr.world->surfacesViewCount[surf] != tr.viewCount)
		{
			tr.world->surfacesViewCount[surf] = tr.viewCount;
			R_AddWorldSurface( tr.world->surfaces + surf, entityNum, tr.currentEntity->needDlights, 0 );
		}
	}
}

void RE_SetRangedFog ( float range )
{
	tr.rangedFog = range;
}


/*
=============================================================

	WORLD MODEL

=============================================================
*/


/*
================
R_RecursiveWorldNode
================
*/
static void R_RecursiveWorldNode( mnode_t *node, int planeBits, int dlightBits, int pshadowBits ) {

	do {
		int			newDlights[2];
		unsigned int newPShadows[2];

		// if the node wasn't marked as potentially visible, exit
		// pvs is skipped for depth shadows
		if (!(tr.viewParms.flags & VPF_DEPTHSHADOW) && node->visCounts[tr.visIndex] != tr.visCounts[tr.visIndex]) {
			return;
		}

		// if the bounding volume is outside the frustum, nothing
		// inside can be visible OPTIMIZE: don't do this all the way to leafs?

		if ( !r_nocull->integer ) {
			int		r;

			if ( planeBits & 1 ) {
				r = BoxOnPlaneSide(node->mins, node->maxs, &tr.viewParms.frustum[0]);
				if (r == 2) {
					return;						// culled
				}
				if ( r == 1 ) {
					planeBits &= ~1;			// all descendants will also be in front
				}
			}

			if ( planeBits & 2 ) {
				r = BoxOnPlaneSide(node->mins, node->maxs, &tr.viewParms.frustum[1]);
				if (r == 2) {
					return;						// culled
				}
				if ( r == 1 ) {
					planeBits &= ~2;			// all descendants will also be in front
				}
			}

			if ( planeBits & 4 ) {
				r = BoxOnPlaneSide(node->mins, node->maxs, &tr.viewParms.frustum[2]);
				if (r == 2) {
					return;						// culled
				}
				if ( r == 1 ) {
					planeBits &= ~4;			// all descendants will also be in front
				}
			}

			if ( planeBits & 8 ) {
				r = BoxOnPlaneSide(node->mins, node->maxs, &tr.viewParms.frustum[3]);
				if (r == 2) {
					return;						// culled
				}
				if ( r == 1 ) {
					planeBits &= ~8;			// all descendants will also be in front
				}
			}

			if ( planeBits & 16 ) {
				r = BoxOnPlaneSide(node->mins, node->maxs, &tr.viewParms.frustum[4]);
				if (r == 2) {
					return;						// culled
				}
				if ( r == 1 ) {
					planeBits &= ~16;			// all descendants will also be in front
				}
			}
		}

		if ( node->contents != -1 ) {
			break;
		}

		// node is just a decision point, so go down both sides
		// since we don't care about sort orders, just go positive to negative

		// determine which dlights are needed
#if 0
		newDlights[0] = 0;
		newDlights[1] = 0;
		if ( dlightBits ) {
			int	i;

			for ( i = 0 ; i < tr.refdef.num_dlights ; i++ ) {
				dlight_t	*dl;
				float		dist;

				if ( dlightBits & ( 1 << i ) ) {
					dl = &tr.refdef.dlights[i];
					dist = DotProduct( dl->origin, node->plane->normal ) - node->plane->dist;
					
					if ( dist > -dl->radius ) {
						newDlights[0] |= ( 1 << i );
					}
					if ( dist < dl->radius ) {
						newDlights[1] |= ( 1 << i );
					}
				}
			}
		}

		newPShadows[0] = 0;
		newPShadows[1] = 0;
		if ( pshadowBits ) {
			int	i;

			for ( i = 0 ; i < tr.refdef.num_pshadows ; i++ ) {
				pshadow_t	*shadow;
				float		dist;

				if ( pshadowBits & ( 1 << i ) ) {
					shadow = &tr.refdef.pshadows[i];
					dist = DotProduct( shadow->lightOrigin, node->plane->normal ) - node->plane->dist;
					
					if ( dist > -shadow->lightRadius ) {
						newPShadows[0] |= ( 1 << i );
					}
					if ( dist < shadow->lightRadius ) {
						newPShadows[1] |= ( 1 << i );
					}
				}
			}
		}
#else
		newDlights[0] = 0;
		newDlights[1] = 0;
		newPShadows[0] = 0;
		newPShadows[1] = 0;
#endif

		// recurse down the children, front side first
		R_RecursiveWorldNode (node->children[0], planeBits, newDlights[0], newPShadows[0] );

		// tail recurse
		node = node->children[1];
		dlightBits = newDlights[1];
		pshadowBits = newPShadows[1];
	} while ( 1 );

	{
		// leaf node, so add mark surfaces
		int			c;
		int surf, *view;

		tr.pc.c_leafs++;

		// add to z buffer bounds
		if ( node->mins[0] < tr.viewParms.visBounds[0][0] ) {
			tr.viewParms.visBounds[0][0] = node->mins[0];
		}
		if ( node->mins[1] < tr.viewParms.visBounds[0][1] ) {
			tr.viewParms.visBounds[0][1] = node->mins[1];
		}
		if ( node->mins[2] < tr.viewParms.visBounds[0][2] ) {
			tr.viewParms.visBounds[0][2] = node->mins[2];
		}

		if ( node->maxs[0] > tr.viewParms.visBounds[1][0] ) {
			tr.viewParms.visBounds[1][0] = node->maxs[0];
		}
		if ( node->maxs[1] > tr.viewParms.visBounds[1][1] ) {
			tr.viewParms.visBounds[1][1] = node->maxs[1];
		}
		if ( node->maxs[2] > tr.viewParms.visBounds[1][2] ) {
			tr.viewParms.visBounds[1][2] = node->maxs[2];
		}

		// add merged and unmerged surfaces
		if (tr.world->viewSurfaces && !r_nocurves->integer)
			view = tr.world->viewSurfaces + node->firstmarksurface;
		else
			view = tr.world->marksurfaces + node->firstmarksurface;

		c = node->nummarksurfaces;
		while (c--) {
			// just mark it as visible, so we don't jump out of the cache derefencing the surface
			surf = *view;
			if (surf < 0)
			{
				if (tr.world->mergedSurfacesViewCount[-surf - 1] != tr.viewCount)
				{
					tr.world->mergedSurfacesViewCount[-surf - 1]  = tr.viewCount;
					//tr.world->mergedSurfacesDlightBits[-surf - 1] = dlightBits;
					//tr.world->mergedSurfacesPshadowBits[-surf - 1] = pshadowBits;
				}
				else
				{
					//tr.world->mergedSurfacesDlightBits[-surf - 1] |= dlightBits;
					//tr.world->mergedSurfacesPshadowBits[-surf - 1] |= pshadowBits;
				}
			}
			else
			{
				if (tr.world->surfacesViewCount[surf] != tr.viewCount)
				{
					tr.world->surfacesViewCount[surf] = tr.viewCount;
					//tr.world->surfacesDlightBits[surf] = dlightBits;
					//tr.world->surfacesPshadowBits[surf] = pshadowBits;
				}
				else
				{
					//tr.world->surfacesDlightBits[surf] |= dlightBits;
					//tr.world->surfacesPshadowBits[surf] |= pshadowBits;
				}
			}
			view++;
		}
	}

}


/*
===============
R_PointInLeaf
===============
*/
static mnode_t *R_PointInLeaf( const vec3_t p ) {
	mnode_t		*node;
	float		d;
	cplane_t	*plane;
	
	if ( !tr.world ) {
		ri->Error (ERR_DROP, "R_PointInLeaf: bad model");
	}

	node = tr.world->nodes;
	while( 1 ) {
		if (node->contents != -1) {
			break;
		}
		plane = node->plane;
		d = DotProduct (p,plane->normal) - plane->dist;
		if (d > 0) {
			node = node->children[0];
		} else {
			node = node->children[1];
		}
	}
	
	return node;
}

/*
==============
R_ClusterPVS
==============
*/
static const byte *R_ClusterPVS (int cluster) {
	if (!tr.world->vis || cluster < 0 || cluster >= tr.world->numClusters ) {
		return NULL;
	}

	return tr.world->vis + cluster * tr.world->clusterBytes;
}

/*
=================
R_inPVS
=================
*/
qboolean R_inPVS( const vec3_t p1, const vec3_t p2, byte *mask ) {
	int		leafnum;
	int		cluster;

	leafnum = ri->CM_PointLeafnum (p1);
	cluster = ri->CM_LeafCluster (leafnum);

	//agh, the damn snapshot mask doesn't work for this
	mask = (byte *) ri->CM_ClusterPVS (cluster);

	leafnum = ri->CM_PointLeafnum (p2);
	cluster = ri->CM_LeafCluster (leafnum);
	if ( mask && (!(mask[cluster>>3] & (1<<(cluster&7)) ) ) )
		return qfalse;

	return qtrue;
}

/*
===============
R_MarkLeaves

Mark the leaves and nodes that are in the PVS for the current
cluster
===============
*/
static void R_MarkLeaves (void) {
	const byte	*vis;
	mnode_t	*leaf, *parent;
	int		i;
	int		cluster;

	// lockpvs lets designers walk around to determine the
	// extent of the current pvs
	if ( r_lockpvs->integer ) {
		return;
	}

	// current viewcluster
	leaf = R_PointInLeaf( tr.viewParms.pvsOrigin );
	cluster = leaf->cluster;

	// if the cluster is the same and the area visibility matrix
	// hasn't changed, we don't need to mark everything again

	for(i = 0; i < MAX_VISCOUNTS; i++)
	{
		// if the areamask or r_showcluster was modified, invalidate all visclusters
		// this caused doors to open into undrawn areas
		if (tr.refdef.areamaskModified || r_showcluster->modified)
		{
			tr.visClusters[i] = -2;
		}
		else if(tr.visClusters[i] == cluster)
		{
			if(tr.visClusters[i] != tr.visClusters[tr.visIndex] && r_showcluster->integer)
			{
				ri->Printf(PRINT_ALL, "found cluster:%i  area:%i  index:%i\n", cluster, leaf->area, i);
			}
			tr.visIndex = i;
			return;
		}
	}

	tr.visIndex = (tr.visIndex + 1) % MAX_VISCOUNTS;
	tr.visCounts[tr.visIndex]++;
	tr.visClusters[tr.visIndex] = cluster;

	if ( r_showcluster->modified || r_showcluster->integer ) {
		r_showcluster->modified = qfalse;
		if ( r_showcluster->integer ) {
			ri->Printf( PRINT_ALL, "cluster:%i  area:%i\n", cluster, leaf->area );
		}
	}

	vis = R_ClusterPVS(tr.visClusters[tr.visIndex]);
	
	for (i=0,leaf=tr.world->nodes ; i<tr.world->numnodes ; i++, leaf++) {
		cluster = leaf->cluster;
		if ( cluster < 0 || cluster >= tr.world->numClusters ) {
			continue;
		}

		// check general pvs
		if ( vis && !(vis[cluster>>3] & (1<<(cluster&7))) ) {
			continue;
		}

		// check for door connection
		if ( (tr.refdef.areamask[leaf->area>>3] & (1<<(leaf->area&7)) ) ) {
			continue;		// not visible
		}

		parent = leaf;
		do {
			if(parent->visCounts[tr.visIndex] == tr.visCounts[tr.visIndex])
				break;
			parent->visCounts[tr.visIndex] = tr.visCounts[tr.visIndex];
			parent = parent->parent;
		} while (parent);
	}
}

qboolean G_BoxInBounds( vec3_t point, vec3_t mins, vec3_t maxs, vec3_t boundsMins, vec3_t boundsMaxs )
{
	vec3_t boxMins;
	vec3_t boxMaxs;

	VectorAdd( point, mins, boxMins );
	VectorAdd( point, maxs, boxMaxs );

	if(boxMaxs[0]>boundsMaxs[0])
		return qfalse;

	if(boxMaxs[1]>boundsMaxs[1])
		return qfalse;

	if(boxMaxs[2]>boundsMaxs[2])
		return qfalse;

	if(boxMins[0]<boundsMins[0])
		return qfalse;

	if(boxMins[1]<boundsMins[1])
		return qfalse;

	if(boxMins[2]<boundsMins[2])
		return qfalse;

	//box is completely contained within bounds
	return qtrue;
}

/*
=============
R_AddWorldSurfaces
=============
*/
extern bool TR_WorldToScreen(vec3_t worldCoord, float *x, float *y);
extern qboolean R_InFOV( vec3_t spot, vec3_t from );

int NUM_WORLD_FOV_CULLS = 0;
int NUM_WORLDMERGED_FOV_CULLS = 0;

void R_AddWorldSurfaces (void) {
	int planeBits;//, dlightBits, pshadowBits;

	if ( !r_drawworld->integer ) {
		return;
	}

	if ( tr.refdef.rdflags & RDF_NOWORLDMODEL ) {
		return;
	}

	NUM_WORLD_FOV_CULLS = 0;
	NUM_WORLDMERGED_FOV_CULLS = 0;

	// determine which leaves are in the PVS / areamask
	if (!(tr.viewParms.flags & VPF_DEPTHSHADOW))
		R_MarkLeaves ();

	// clear out the visible min/max
	ClearBounds( tr.viewParms.visBounds[0], tr.viewParms.visBounds[1] );

	// perform frustum culling and flag all the potentially visible surfaces
	tr.refdef.num_dlights = min (tr.refdef.num_dlights, 32) ;
	//tr.refdef.num_pshadows = min (tr.refdef.num_pshadows, 32) ;

	planeBits = (tr.viewParms.flags & VPF_FARPLANEFRUSTUM) ? 31 : 15;

	/*
	if ( tr.viewParms.flags & VPF_DEPTHSHADOW )
	{
		dlightBits = 0;
		pshadowBits = 0;
	}
	else if ( !(tr.viewParms.flags & VPF_SHADOWMAP) )
	{
		dlightBits = ( 1 << tr.refdef.num_dlights ) - 1;
		pshadowBits = ( 1 << tr.refdef.num_pshadows ) - 1;
	}
	else
	{
		dlightBits = ( 1 << tr.refdef.num_dlights ) - 1;
		pshadowBits = 0;
	}
	*/

	R_RecursiveWorldNode( tr.world->nodes, planeBits, 0/*dlightBits*/, 0/*pshadowBits*/);

	// now add all the potentially visible surfaces
	// also mask invisible dlights for next frame
	{
		int i;

		//tr.refdef.dlightMask = 0;

		for (i = 0; i < tr.world->numWorldSurfaces; i++)
		{
			if (tr.world->surfacesViewCount[i] != tr.viewCount)
				continue;

			if (r_fovCull->integer && backEnd.viewParms.targetFbo != tr.renderCubeFbo)
			{
				vec3_t bounds[2];

				bounds[0][0] = (tr.world->surfaces + i)->cullinfo.bounds[0][0];
				bounds[0][1] = (tr.world->surfaces + i)->cullinfo.bounds[1][0];
				bounds[0][2] = (tr.world->surfaces + i)->cullinfo.bounds[0][2];

				bounds[1][0] = (tr.world->surfaces + i)->cullinfo.bounds[1][0];
				bounds[1][1] = (tr.world->surfaces + i)->cullinfo.bounds[0][0];
				bounds[1][2] = (tr.world->surfaces + i)->cullinfo.bounds[1][2];

				if (!R_InFOV((tr.world->surfaces + i)->cullinfo.bounds[0], tr.refdef.vieworg)
					&& !R_InFOV((tr.world->surfaces + i)->cullinfo.bounds[1], tr.refdef.vieworg)
					&& !R_InFOV(bounds[0], tr.refdef.vieworg)
					&& !R_InFOV(bounds[1], tr.refdef.vieworg)) {
						NUM_WORLD_FOV_CULLS++;
						continue;
				}
			}

			//R_AddWorldSurface( tr.world->surfaces + i, 0/*tr.world->surfacesDlightBits[i]*/, 0/*tr.world->surfacesPshadowBits[i]*/ );
			R_AddWorldSurface( tr.world->surfaces + i, REFENTITYNUM_WORLD, 0, 0 );
			//tr.refdef.dlightMask |= tr.world->surfacesDlightBits[i];
		}

		for (i = 0; i < tr.world->numMergedSurfaces; i++)
		{
			if (tr.world->mergedSurfacesViewCount[i] != tr.viewCount)
				continue;

#if 0 // Ignore this - I have not seen it remove anything, no point wasting time checking...
			if (r_fovCull->integer && backEnd.viewParms.targetFbo != tr.renderCubeFbo)
			{
				vec3_t bounds[2];

				bounds[0][0] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[0][0];
				bounds[0][1] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[1][0];
				bounds[0][2] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[0][2];

				bounds[1][0] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[1][0];
				bounds[1][1] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[0][0];
				bounds[1][2] = (tr.world->mergedSurfaces + i)->cullinfo.bounds[1][2];

				if (!R_InFOV((tr.world->mergedSurfaces + i)->cullinfo.bounds[0], tr.refdef.vieworg)
					&& !R_InFOV((tr.world->mergedSurfaces + i)->cullinfo.bounds[1], tr.refdef.vieworg)
					&& !R_InFOV(bounds[0], tr.refdef.vieworg)
					&& !R_InFOV(bounds[1], tr.refdef.vieworg)) {
					NUM_WORLDMERGED_FOV_CULLS++;
					continue;
				}
			}
#endif

			//R_AddWorldSurface( tr.world->mergedSurfaces + i, 0/*tr.world->mergedSurfacesDlightBits[i]*/, 0/*tr.world->mergedSurfacesPshadowBits[i]*/ );
			R_AddWorldSurface( tr.world->mergedSurfaces + i, REFENTITYNUM_WORLD, 0, 0 );
			//tr.refdef.dlightMask |= tr.world->mergedSurfacesDlightBits[i];
		}

		//tr.refdef.dlightMask = ~tr.refdef.dlightMask;

		if (r_fovCull->integer >= 2)
			ri->Printf(PRINT_WARNING, "Culled %i world surfaces. %i World and %i Merged.\n", NUM_WORLD_FOV_CULLS + NUM_WORLDMERGED_FOV_CULLS, NUM_WORLD_FOV_CULLS, NUM_WORLDMERGED_FOV_CULLS);
	}
}
