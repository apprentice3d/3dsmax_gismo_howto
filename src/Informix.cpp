//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: Denis Grigor
//***************************************************************************/

#include "Informix.h"
#include <string>

#define Informix_CLASS_ID	Class_ID(0x325904c0, 0x632c9d0e)


#define PBLOCK_REF  SIMPMOD_PBLOCKREF




class Informix : public SimpleMod2
{
	public:
		//Constructor/Destructor
		Informix();
		~Informix();

		// Parameter block handled by parent

		// From BaseObject
		virtual const TCHAR* GetObjectName() { return GetString(IDS_CLASS_NAME); }

		//From Modifier

		virtual void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		virtual void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);

		// From SimpleMod
		virtual Deformer& GetDeformer(TimeValue t,ModContext &mc,Matrix3& mat,Matrix3& invmat);

		virtual Interval GetValidity(TimeValue t);

		// Loading/Saving
		virtual IOResult Load(ILoad *iload);
		virtual IOResult Save(ISave *isave);


	//From Animatable
		virtual Class_ID ClassID() {return Informix_CLASS_ID;}
		virtual SClass_ID SuperClassID() { return OSM_CLASS_ID; }
		virtual void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

		virtual RefTargetHandle Clone( RemapDir &remap );

		virtual int NumParamBlocks() { return 1; }						// return number of ParamBlocks in this instance
		virtual IParamBlock2* GetParamBlock(int /*i*/) { return pblock2; }	// return i'th ParamBlock
		virtual IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock2->ID() == id) ? pblock2 : NULL; } // return id'd ParamBlock

		virtual void DeleteThis() { delete this; }


		int Display(TimeValue t, INode* inode, ViewExp* vpt, int flagst, ModContext* mc) override;

private:
		void makeWingsidePoints(Point3 reference_point, Point3& wing_side_point, Point3& wing_other_side_point, float arrow_wing_factor, const Point3 source);
		void drawMyArrows(Point3 p1, Point3 p2, const wchar_t* my_text, ViewExp* vpt);
};

//This is the callback object used by modifiers to deform "Deformable" objects.
class InformixDeformer : public Deformer
{
public:
	InformixDeformer();
	//TODO: Add other plug-in specific constructors and member functions
	Point3 Map(int i, Point3 p); 
};



class InformixClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new Informix(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return OSM_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return Informix_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("Informix"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetInformixDesc() { 
	static InformixClassDesc InformixDesc;
	return &InformixDesc; 
}




enum { informix_params };


//TODO: Add enums for various parameters
enum { 
	pb_wing_spin,
};




static ParamBlockDesc2 informix_param_blk ( informix_params, _T("params"),  0, GetInformixDesc(), 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	pb_wing_spin, 			_T("wing_spin"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_SPIN, 
		p_default, 		0.5f, 
		p_range, 		0.0f,10.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_EDIT,	IDC_SPIN, 0.01f, 
		p_end,
	p_end
	);






InformixDeformer::InformixDeformer() 
{ 
	
}

/*************************************************************************************************
*
 	Map is called for every deformable point in the object
*
\*************************************************************************************************/

Point3 InformixDeformer::Map(int /*i*/, Point3 p)
{
	//TODO: Add code to deform or alter a single point
	return p;
}

//--- Informix -------------------------------------------------------
Informix::Informix()
{
	pblock2 = nullptr;
	GetInformixDesc()->MakeAutoParamBlocks(this);
}

Informix::~Informix()
{
}


void Informix::BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev )
{
	TimeValue t = ip->GetTime();
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_BEGIN_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_ON);
	SetAFlag(A_MOD_BEING_EDITED);

	SimpleMod2::BeginEditParams(ip,flags,prev);
	GetInformixDesc()->BeginEditParams(ip, this, flags, prev);
}

void Informix::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next)
{
	SimpleMod2::EndEditParams(ip,flags,next);
	GetInformixDesc()->EndEditParams(ip, this, flags, next);

	TimeValue t = ip->GetTime();
	ClearAFlag(A_MOD_BEING_EDITED);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_END_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_OFF);
}



Interval Informix::GetValidity(TimeValue /*t*/)
{
	Interval valid = FOREVER;
	//TODO: Return the validity interval of the modifier
	return valid;
}

Deformer& Informix::GetDeformer( TimeValue /*t*/, ModContext& /*mc*/, Matrix3& /*mat*/, Matrix3& /*invmat*/)
{
	static InformixDeformer deformer;
	//TODO: Add code to modify the deformer object
	return deformer;
}



RefTargetHandle Informix::Clone(RemapDir& remap)
{
	Informix* newmod = new Informix();	
	//TODO: Add the cloning code here
	newmod->ReplaceReference(PBLOCK_REF,remap.CloneRef(pblock2));
	newmod->SimpleModClone(this, remap);
	BaseClone(this, newmod, remap);
	return(newmod);
}

IOResult Informix::Load(ILoad* /*iload*/)
{
	//TODO: Add code to allow plugin to load its data
	return IO_OK;
}

IOResult Informix::Save(ISave* /*isave*/)
{
	//TODO: Add code to allow plugin to save its data
	return IO_OK;
}

void Informix::makeWingsidePoints(Point3 reference_point, Point3 &wing_side_point, Point3 &wing_other_side_point, float arrow_wing_factor, const Point3 source)
{
		wing_side_point = reference_point;
		wing_other_side_point = wing_side_point;
	
		if(reference_point.x != source.x)
		{
			wing_side_point.z -= arrow_wing_factor;
			wing_other_side_point.z += arrow_wing_factor;
		}
		else
		{
			wing_side_point.x -= arrow_wing_factor;
			wing_other_side_point.x += arrow_wing_factor;
		}
		wing_side_point.y -= arrow_wing_factor;
		wing_other_side_point.y += arrow_wing_factor;
}

void Informix::drawMyArrows(Point3 p1, Point3 p2, const wchar_t* my_text, ViewExp* vpt)
{
	float arrow_scale_factor = 0.05;
	float arrow_wing_factor = 10.0f; // max(max(p1.z + p2.z, p1.x + p2.x)*arrow_scale_factor, 1);
	pblock2->GetValue(pb_wing_spin, GetCOREInterface()->GetTime(), arrow_wing_factor, FOREVER);
	Point3 wing_side_point(0,0,0);
	Point3 wing_other_side_point(0,0,0);
	Point3 ref_point((p1.x + p2.x), (p1.y + p2.y), (p1.z + p2.z));
	Point3 midpoint = ref_point / 2;
	
	//prepare point for left arrow
	Point3 left_reference_point = p1*(1-arrow_scale_factor)+p2*arrow_scale_factor;
	makeWingsidePoints(left_reference_point, wing_side_point, wing_other_side_point, arrow_wing_factor, p1);
	Point3 my_gizmo_point_left[5] = { p2, p1, wing_side_point, p1, wing_other_side_point };

	//prepare point for right arrow
	Point3 right_reference_point = p1*arrow_scale_factor + p2*(1 - arrow_scale_factor);
	makeWingsidePoints(right_reference_point, wing_side_point, wing_other_side_point, arrow_wing_factor, p1);
	Point3 my_gizmo_point_right[5] = { p1, p2, wing_side_point, p2, wing_other_side_point };

	//draw points and labels
	GraphicsWindow *gw = vpt->getGW();
	DrawLineProc lp(gw);
	lp.SetLineColor(0, 0, 0);
	lp.proc(my_gizmo_point_left, 5);
	lp.proc(my_gizmo_point_right, 5);
	gw->setColor(TEXT_COLOR, 0, 0, 0);
	gw->text(&midpoint, my_text);

}



int Informix::Display(TimeValue t, INode* inode, ViewExp* vpt, int flagst, ModContext* mc)
{
	TimeValue current_time = GetCOREInterface()->GetTime();
	Box3 bounding_box;
	inode->EvalWorldState(current_time).obj->GetLocalBoundBox(current_time, inode,vpt,bounding_box);
	Point3 min = bounding_box.Min();
	Point3 max = bounding_box.Max();
	Point3 x_max(max.x, min.y, min.z);
	Point3 y_max(min.x, max.y, min.z);
	Point3 z_max(min.x, min.y, max.z);

	std::wstring label_x = std::to_wstring(max.x - min.x) + L" units";
	std::wstring label_y = std::to_wstring(max.y - min.y) + L" units";
	std::wstring label_z = std::to_wstring(max.z - min.z) + L" units";

	drawMyArrows(min, x_max, label_x.c_str(), vpt);
	drawMyArrows(min, y_max, label_y.c_str(), vpt);
	drawMyArrows(min, z_max, label_z.c_str(), vpt);
	
	return 0;
}
