#include "exifloader.h"

#include <exiv2/image.hpp>

#include <QDebug>

QString parseExiv2Value (const Exiv2::Exifdatum& value)
{
    return QString::fromStdString( value.toString() );
}

ExifLoader::ExifLoader()
{
}

QMap <QString, QVariant> ExifLoader::loadInfo (QString path)
{
    QMap <QString, QVariant> info;

    try
    {

        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open( path.toStdString() );

        image->readMetadata();

        Exiv2::ExifData exif_data = image->exifData();

        if (exif_data.count())
        {

//            for (Exiv2::ExifData::iterator it = exif_data.begin(); it != exif_data.end(); it++)
//            {
//                qDebug() << QString::fromStdString(it->key()) << QString::fromStdString(it->value().toString());
//            }



            info["model"]    = parseExiv2Value (exif_data["Exif.Image.Model"] );
            info["make"]     = parseExiv2Value (exif_data["Exif.Image.Make"] );
            info["aperture"] = parseExiv2Value (exif_data["Exif.Photo.FNumber"] );
            info["focal_len"] = parseExiv2Value (exif_data["Exif.Photo.FocalLength"] );
            info["iso_speed"] = parseExiv2Value (exif_data["Exif.Photo.ISOSpeedRatings"] );
            info["shutter"] = parseExiv2Value (exif_data["Exif.Photo.ExposureTime"] );

            info["timestamp"] = parseExiv2Value (exif_data["Exif.Image.DateTime"] );
            info["shot_order"] = parseExiv2Value (exif_data["Exif.Sony2.SequenceNumber"] );

            info["lens"] = parseExiv2Value (exif_data["Exif.Photo.LensModel"] );

            info["orientation"] = parseExiv2Value (exif_data["Exif.Image.Orientation"] );

                qDebug() <<  info;
        }

    } catch (Exiv2::AnyError& e) {
        std::cout << "Caught Exiv2 exception '" << e << "'\n";
    }

    return info;

}
