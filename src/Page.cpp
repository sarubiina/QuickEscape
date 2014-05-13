// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licensed under GPLv3.
#include <Page.h>
#include <string>
#include <stdexcept>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
Page::Page() : _pageFont(NULL), _full(false), _justification(Justification::Justified) 
{
  
}
////////////////////////////////////////////////////////////////////////////////
Page::~Page()
{
  SDL_DestroyTexture(_pageTexture); 
}
////////////////////////////////////////////////////////////////////////////////
void
Page::Init( SDL_Renderer * renderer, size_t width, size_t height )
{
  _pageTexture = SDL_CreateTexture( renderer, 
				    SDL_PIXELFORMAT_RGBA8888, 
				    SDL_TEXTUREACCESS_TARGET, 
				    width, height);
  _pageHeight = 0;
  _pageMaxWidth = width;
  _pageMaxHeight = height;
  if ( !_pageTexture )  throw runtime_error(SDL_GetError());
}
////////////////////////////////////////////////////////////////////////////////
void 
Page::Compose( TTF_Font *font )
{
  // page font must exist
  if ( (_pageFont = font) == NULL ) throw runtime_error("Page font not set! (NULL)");
  
  // recommended line height
  size_t lineSkip = TTF_FontLineSkip(_pageFont);
  
  stringstream & copy = *this;
  
  while(copy.good())
  {
    // if we have exceeded _pageHeight.
    if ( _pageHeight+lineSkip > _pageMaxHeight)
    {	

      if ( fillMode_ == FillMode::Head )
      {
	// extract remaining string 
	string remaining;
	std::getline(copy,remaining);
	// remove remaining string
	copy.str( copy.str().substr(0, copy.str().size()-remaining.size()));
	copy.seekg(ios_base::end);
	copy.seekp(ios_base::end);
	// throw remainder within exception
	throw PageFullException(remaining);
      }
      else // FillMode::Tail
      {
	// remove first line so new ones fit.
	_lines.pop_front();
	_pageHeight-=lineSkip;
      }
      
    }
    // extract words until page line is full.
    string word;
    stringstream line;      
    int lineWidth = 0;
    int lineHeight = 0;

    // extract new word
    copy >> word;
    line << word;
    
    // if word is empty, we are done.
    if ( line.str().size() == 0 ) break;

    // Compute dimensions of line if rendered
    if ( TTF_SizeUTF8(_pageFont, line.str().c_str(), &lineWidth, &lineHeight) )
    {
      throw runtime_error(TTF_GetError());
    }
    // for rewinding our stream
    streampos p;

    // extract words until line is full
    while( copy.good() && (size_t)lineWidth < _pageMaxWidth)
    {
      p = copy.tellg();
      copy >> word;
      line << " " << word; // add whitespace between words
      if ( TTF_SizeUTF8(_pageFont, line.str().c_str(), &lineWidth, &lineHeight) )
	throw runtime_error(TTF_GetError());
      //cout << line.str() << " linewidth now: " << lineWidth << "\n";
    } 
    
    // make next extraction get last word that did not fit.
    if ( (size_t)lineWidth >= _pageMaxWidth )  copy.seekg(p);
      
    
    
    // get number of characters in line
    size_t len = line.str().size();
    // remove last word from line.
    line.str(line.str().substr(0,len-word.size()-1));
    //cout << "adding line: " << line.str() << "\n";

    // Append line to rendered content
    _lines.push_back(line.str());
    // increase current page height by lineskip
    _pageHeight += lineSkip;
  }
  copy.clear();

}
////////////////////////////////////////////////////////////////////////////////
void 
Page::RenderContent( SDL_Renderer *renderer)
{

  if ( _pageTexture == NULL ) throw runtime_error("Page texture not set!");
  SDL_SetTextureBlendMode(_pageTexture, SDL_BLENDMODE_BLEND);
  // prepare renderer to render into page texture
  SDL_SetRenderTarget(renderer, _pageTexture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  
  // line height will be this
  int lineSkip = TTF_FontLineSkip(_pageFont);

  int lineNumber = 0;
  // for each line in list
  //for( const string & line : _lines )
  for( list<string>::iterator l = _lines.begin(); l!= _lines.end(); l++)
  {
    const string & line = *l;
    //cout << "going to rendering: '" << line << "'\n";

    // Render with SDL_ttf
    SDL_Color c = { 0,0,0,255};
    SDL_Surface *text = TTF_RenderText_Blended( _pageFont, line.c_str(), c);
    if ( !text ) throw runtime_error( TTF_GetError());
    int textW = text->w;
    int textH = text->h;
    //cout << "Rendered surface width: " << text->w << "\n";

    // Convert into texture
    SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer,text);
    if ( !text_tex ) throw runtime_error( SDL_GetError());
    // Release unnecessary surface
    SDL_FreeSurface(text);
    //  Compute 
    SDL_Rect target = {0, lineNumber*lineSkip, textW, textH};
    // justify text 
    switch ( _justification )
    {
    case Justification::Right:
      target.x = _pageMaxWidth - textW;
      break;
    case Justification::Center:
      target.x = (_pageMaxWidth - textW)/2;
      break;
    case Justification::Justified:
      target.w = _pageMaxWidth;
      break;
    case Justification::Left:
      default:
	break;
    }
    // blit generated text to our page texture 
    SDL_RenderCopy(renderer, text_tex, NULL, &target);
    // free temp texture
    SDL_DestroyTexture(text_tex);
    lineNumber++;
  }
  // Reset render target to be sure
  SDL_SetRenderTarget(renderer, NULL);
}
////////////////////////////////////////////////////////////////////////////////
void
Page::Clear()
{
  str("");
}
////////////////////////////////////////////////////////////////////////////////
bool
Page::IsDirty() const
{
  return dirty_;
}
////////////////////////////////////////////////////////////////////////////////
void
Page::SetDirty(bool flag )
{
  dirty_ = flag;
}
////////////////////////////////////////////////////////////////////////////////
