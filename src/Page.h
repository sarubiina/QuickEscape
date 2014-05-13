// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#ifndef __Page_h__
#define __Page_h__
////////////////////////////////////////////////////////////////////////////////
#include <list>
#include <string>
#include <sstream>
#include <stdexcept>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
namespace Justification 
{
  enum Justification { Left, Right, Center, Justified, kNumJustification };
}
////////////////////////////////////////////////////////////////////////////////
namespace Margin 
{
  enum Margin { Left, Right, Top, Bottom, kNumMargin } ;
}
////////////////////////////////////////////////////////////////////////////////
namespace Scaling 
{
  enum Scaling { Horizontal = 1,
		 Vertical = 2,
		 Both = 3,
		 kNumScaling
  };
}
namespace FillMode 
{
  enum FillMode { 
    Head, // displayes only first lines that fit page (throws exception when page full).
    Tail, // discards lines silently from beginning to fit new ones.
    kNumFillMode
  };
}
////////////////////////////////////////////////////////////////////////////////
class PageFullException : public std::runtime_error 
{
public:
  PageFullException( const std::string & msg ) : std::runtime_error(msg){}
  //using std::runtime_error::runtime_error; 
};
////////////////////////////////////////////////////////////////////////////////
const size_t DEFAULT_PAGE_TEXTURE_WIDTH = 512;
const size_t DEFAULT_PAGE_TEXTURE_HEIGHT = 1024;
////////////////////////////////////////////////////////////////////////////////
class Page : public std::stringstream
{
public:
  size_t			_pageMaxWidth;
  size_t			_pageMaxHeight;
  size_t			_pageHeight;
  SDL_Texture *			_pageTexture;
  std::list<std::string>	_lines;
  TTF_Font *			_pageFont;
  bool				_full;
  size_t			_pageMargin[Margin::kNumMargin];
  bool				dirty_;
  Justification::Justification _justification;
  FillMode::FillMode	       fillMode_ { FillMode::Tail } ;
  Page();
  virtual ~Page();
  void Init( SDL_Renderer *renderer,  
	     size_t width = DEFAULT_PAGE_TEXTURE_WIDTH, 
	     size_t height = DEFAULT_PAGE_TEXTURE_HEIGHT);
  // Updates texture 
  void RenderContent( SDL_Renderer *renderer);
  // Breaks content into appropriate lines.
  void Compose( TTF_Font *font );
  void Clear();
  bool IsDirty() const;
  void SetDirty( bool flag );
};
////////////////////////////////////////////////////////////////////////////////
template<class T>
Page & operator<<(Page & p, T & t )
{
  static_cast<std::stringstream&>(p) << t;
  return p;
}
////////////////////////////////////////////////////////////////////////////////
#endif
