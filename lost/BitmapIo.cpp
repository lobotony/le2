#include "lost/BitmapIo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

namespace lost
{

struct BitmapMmapDeleter
{
  void operator()(Bitmap* p) const
  {
    if(p)
    {
      if(p->mmapaddr)
      {
        ASSERT(munmap(p->mmapaddr, p->mmapsize)!=-1, "error unmapping file");
      }
      delete p;
    }
  }
};


BitmapPtr loadMapped(const string& filepath)
{
  shared_ptr<Bitmap> result(new Bitmap, BitmapMmapDeleter());
  int fd = open(filepath.c_str(), O_RDONLY);
  ASSERT(fd != -1, "file open failed: "<<filepath<<" : "<<strerror(errno));
  struct stat sb;
  int err = fstat(fd, &sb);
  ASSERT(err != -1, "fstat failed");
  result->mmapaddr = (u8*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  ASSERT(close(fd)!=-1, "close failed");
  result->mmapsize = (u32)sb.st_size;
  BitmapRawHeader* header = (BitmapRawHeader* )result->mmapaddr;
  result->width = header->width;
  result->height = header->height;
  switch(header->bytesPerPixel)
  {
    case 1:result->format = GL_ALPHA;break;
    case 3:result->format = GL_RGB;break;
    case 4:result->format = GL_RGBA;break;
    default:ASSERT(false, "unknown bytesPerPixel: "<<header->bytesPerPixel);break;
  }
  result->premultiplied = header->premultipliedAlpha;
  result->data = result->mmapaddr+sizeof(BitmapRawHeader);

  return result;
}

void writeRawToPath(const BitmapPtr& bmp, const string& filepath)
{
  int fd = open(filepath.c_str(), O_CREAT| O_WRONLY, S_IRUSR|S_IWUSR);
  BitmapRawHeader header;
  header.width = bmp->width;
  header.height = bmp->height;
  switch(bmp->format)
  {
    case GL_ALPHA:header.bytesPerPixel=1;break;
    case GL_RGB:header.bytesPerPixel=3;break;
    case GL_RGBA:header.bytesPerPixel=4;break;
    default:ASSERT(false, "unknown format:"<<bmp->format);break;
  }
  header.premultipliedAlpha = bmp->premultiplied ? 1 : 0;
  ASSERT(write(fd, &header, sizeof(header))!=-1, "header write failed");
  ASSERT(write(fd, bmp->data, header.width*header.height*header.bytesPerPixel)!=-1, "body write failed");
  ASSERT(close(fd)!=-1, "close failed");
}


}