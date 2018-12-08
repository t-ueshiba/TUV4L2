/*
 *  $Id$
 */
#include "TU/V4L2CameraArray.h"
#include "TU/io.h"

namespace TU
{
#ifndef TUV4L2PP_CONF_DIR
#  define TUV4L2PP_CONF_DIR	"/usr/local/etc"
#endif
    
/************************************************************************
*  class V4L2CameraArray						*
************************************************************************/
constexpr const char*	V4L2CameraArray::DEFAULT_CAMERA_NAME;
    
//! 空のVideo for Linux v.2カメラの配列を生成する.
/*!
  \param name		カメラ名
*/
V4L2CameraArray::V4L2CameraArray(const char* name)
    :Array<V4L2Camera>(), _name(name)
{
}
    
//! 設定ファイルを読み込んでVideo for Linux v.2カメラの配列を初期化する.
void
V4L2CameraArray::restore()
{
    std::ifstream	in(configFile().c_str());
    if (!in)
	throw std::runtime_error("V4L2CameraArray::restore(): cannot open " +
				 configFile());
    in >> *this;
}

//! Video for Linux v.2カメラの配列の設定を設定ファイルに書き込む.
void
V4L2CameraArray::save() const
{
    std::ofstream	out(configFile().c_str());
    if (!out)
	throw std::runtime_error("V4L2CameraArray::save(): cannot open " +
				 configFile());
    out << *this;
}
    
//! カメラ設定ファイル名を返す.
/*!
  \return	カメラ設定ファイル名
*/
std::string
V4L2CameraArray::configFile() const
{
    return std::string(TUV4L2PP_CONF_DIR) + '/' + _name + ".conf";
}
    
//! キャリブレーションファイル名を返す.
/*!
  \return	キャリブレーションファイル名
*/
std::string
V4L2CameraArray::calibFile() const
{
    return std::string(TUV4L2PP_CONF_DIR) + '/' + _name + ".calib";
}

/************************************************************************
*  global functions							*
************************************************************************/
std::ostream&
operator <<(std::ostream& out, const V4L2CameraArray& cameras)
{
    YAML::Emitter	emitter;
    emitter << YAML::BeginSeq;
    for (const auto& camera : cameras)
	emitter << camera;
    emitter << YAML::EndSeq;

    return out << emitter.c_str() << std::endl;
}

std::istream&
operator >>(std::istream& in, V4L2CameraArray& cameras)
{
    const auto	node = YAML::Load(in);
    cameras.resize(node.size());
    for (size_t i = 0; i < node.size(); ++i)
	node[i] >> cameras[i];

    return in;
}

}
