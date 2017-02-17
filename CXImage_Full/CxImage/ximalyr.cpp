// xImaLyr.cpp : Layers functions
/* 21/04/2003 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

#if CXIMAGE_SUPPORT_LAYERS

////////////////////////////////////////////////////////////////////////////////
/**
 * If the object is an internal layer, GetParent return its parent in the hierarchy.
 */
CxImage* CxImage::GetParent() const
{
	return m_stImgInfo.pParent;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Number of layers allocated directly by the object.
 */
long CxImage::GetNumLayers() const
{
	return m_stImgInfo.nNumLayers;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Creates an empty layer. If position is less than 0, the new layer will be placed in the last position
 */
bool CxImage::LayerCreate(long position)
{
	if ( position < 0 || position > m_stImgInfo.nNumLayers ) position = m_stImgInfo.nNumLayers;

	CxImage** ptmp = new CxImage*[m_stImgInfo.nNumLayers + 1];
	if (ptmp==0) return false;

	int i=0;
	for (int n=0; n<m_stImgInfo.nNumLayers; n++){
		if (position == n){
			ptmp[n] = new CxImage();
			i=1;
		}
		ptmp[n+i]=m_ppLayers[n];
	}
	if (i==0) ptmp[m_stImgInfo.nNumLayers] = new CxImage();

	if (ptmp[position]){
		ptmp[position]->m_stImgInfo.pParent = this;
	} else {
		free(ptmp);
		return false;
	}

	m_stImgInfo.nNumLayers++;
	delete [] m_ppLayers;
	m_ppLayers = ptmp;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Deletes a layer. If position is less than 0, the last layer will be deleted
 */
bool CxImage::LayerDelete(long position)
{
	if ( position >= m_stImgInfo.nNumLayers ) return false;
	if ( position < 0) position = m_stImgInfo.nNumLayers - 1;
	if ( position < 0) return false;

	if (m_stImgInfo.nNumLayers>1){

		CxImage** ptmp = new CxImage*[m_stImgInfo.nNumLayers - 1];
		if (ptmp==0) return false;

		int i=0;
		for (int n=0; n<m_stImgInfo.nNumLayers; n++){
			if (position == n){
				delete m_ppLayers[n];
				i=1;
			}
			ptmp[n]=m_ppLayers[n+i];
		}

		m_stImgInfo.nNumLayers--;
		delete [] m_ppLayers;
		m_ppLayers = ptmp;

	} else {
		delete m_ppLayers[0];
		delete [] m_ppLayers;
		m_ppLayers = 0;
		m_stImgInfo.nNumLayers = 0;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::LayerDeleteAll()
{
	if (m_ppLayers) { 
		for(long n=0; n<m_stImgInfo.nNumLayers;n++){ delete m_ppLayers[n]; }
		delete [] m_ppLayers; m_ppLayers=0; m_stImgInfo.nNumLayers = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns a pointer to a layer. If position is less than 0, the last layer will be returned
 */
CxImage* CxImage::GetLayer(long position)
{
	if ( m_ppLayers == NULL) return NULL;
	if ( m_stImgInfo.nNumLayers == 0) return NULL;
	if ( position >= m_stImgInfo.nNumLayers ) return NULL;
	if ( position < 0) position = m_stImgInfo.nNumLayers - 1;
	return m_ppLayers[position];
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_LAYERS
