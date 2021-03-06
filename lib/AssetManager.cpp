#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::add(char * key, Asset * asset)
{
    if( !this->contains(key) )
        #ifdef TILE2D_VERBOSE_OUTPUT
        std::cout << "Adding " << key << " to AssetManager." << std::endl;
        #endif
        this->assetHash.insert(std::pair<std::string,Asset*>(key,asset));
    #ifdef TILE2D_VERBOSE_OUTPUT
    else std::cout << "Adding " << key << " to AssetManager." << std::endl;
    #endif
}

tex_error AssetManager::addNewTexture(char * key, char * filepath)
{
    Texture * t = new Texture();
    tex_error e;
    if(filepath == NULL) e = t->createEmpty();
    else e = t->load(filepath);
    if( e == TEX_NO_ERROR ) this->add(key,(Asset*)t);
    return e;
}

tex_error AssetManager::addNewTexture(const char * key, const char * filepath)
{
    return this->addNewTexture((char*)key, (char*)filepath);
}

shader_error AssetManager::addNewShader(char * key, char * vertPath, char * fragPath)
{
    Shader * s = new Shader();
    shader_error e = s->load(vertPath, fragPath);
    if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
    return e;
}

shader_error AssetManager::addNewShader(const char * key, const char * vertPath, const char * fragPath)
{
    return this->addNewShader((char*)key, (char*)vertPath, (char*)fragPath);
}

bool AssetManager::contains(char * key)
{    
    // The ol' annoying iterator check.
    return ( this->assetHash.find(key) != this->assetHash.end() );
}

Asset * AssetManager::get(char * key)
{
    if( this->contains(key) )
    {
        return this->assetHash[key];
    }
    else return NULL;
}

bool AssetManager::remove(char * key)
{
    if( this->contains(key) )
    {
        this->assetHash[key]->destroy();
        delete (this->assetHash[key]);
        return true;
    }
    return false;
}

void AssetManager::clear()
{
    // This is super ugly.
    for( std::map<std::string, Asset*>::iterator i = this->assetHash.begin(); 
         i != this->assetHash.end();
         ++i ) {
        
        // Destroy the Asset.
        i->second->destroy();
        // Delete the asset instance.
        delete i->second;
    }
    
    // Now that all the asset hash contains has been deleted, we can
    // clear out the underlying map.
    this->assetHash.erase(this->assetHash.begin(), this->assetHash.end());
}