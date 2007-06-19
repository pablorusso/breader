#include "Tag.h"
#include "XmlUtils.h"

Tag::Tag(){};

Tag::~Tag() {}


string Tag::getXML()
{
	return "<tag id=\"" + XmlUtils::xmlEncode( idTag )+ "\" readOnly=\"" + XmlUtils::xmlEncode( isReadOnly ) + "\" name=\"" + XmlUtils::xmlEncode( tagName ) + "\" artCount=\"" + XmlUtils::xmlEncode( artCount ) + "\"/>";
}

void Tag::ConvertToTag(t_regArchivo4 reg)
{
	
	this->idTag = reg.idCategory;
	this->tagName = reg.categoryName;
	this->artCount = reg.artPositive;
	if(this->idTag==IDCAT_FAV)	
			this->isReadOnly = "true";
		else
			this->isReadOnly = "false";
}

/*


t_idcat Tag::createTag(const string &tagName)
{
	
	t_idcat ret;
	try 
	{	
		ret = this->a4.addCategory(tagName);
	}
	catch (IException &e) {
// 		eTag mie(e.getErrorMensaje());
		throw(mie);
	}
	return ret;
}

void Tag::getTag(t_idcat idTag)
{
	try
	{	
		t_Tag regCat;
		regCat =  this->a4.getTagInfo(idTag);
		this->id = XmlUtils::uintToStr(regCat.idTag);
		this->artCount = XmlUtils::uintToStr(regCat.artCount);
		this->name = regCat.tagName;
		if(idTag==IDCAT_FAV)	
			this->isReadOnly = "true";
		else
			this->isReadOnly = "false";
	}catch(IException &e) {
		eTag mie(e.getErrorMensaje());
		throw(mie);
	}
}

void Tag::updateTag(t_idcat id, string tagName)
{
	try
	{
		this->a4.modifyCategoryName(id, tagName);
	
	}catch(IException &e) {
		eTag mie(e.getErrorMensaje());
		throw(mie);
	}
}

void Tag::deleteTag(t_idcat id )
{
	try
	{
		if(id == IDCAT_FAV)
		{
			eTag mie("No puede borrar la categoria favoritos");
			throw(mie);
		}else
		{
 			this->a4.deleteCategory(id);
		}
		
	}catch(IException &e) {
		eTag mie(e.getErrorMensaje());
		throw(mie);
	}
}

t_queue_idcat Tag::getAllTags()
{
	try
	{
		return this->a4.getCategoriesId();
		
	}catch(IException &e) {
		eTag mie(e.getErrorMensaje());
		throw(mie);
	}
}*/
