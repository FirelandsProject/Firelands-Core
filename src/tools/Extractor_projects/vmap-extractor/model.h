/*
 * This file is part of the Firelands Core Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vec3d.h"
#include "modelheaders.h"
#include "vmapexport.h"

class WMOInstance;
class MPQFile;

Vec3D fixCoordSystem(Vec3D v);

/**
 * @brief
 *
 */
class Model
{
    public:
        ModelHeader header;
        uint32 offsBB_vertices, offsBB_indices;
        Vec3D* BB_vertices, *vertices;
        uint16* BB_indices, *indices;
        size_t nIndices; /**< TODO */

        /**
         * @brief
         *
         * @param failedPaths
         * @return bool
         */
        bool open(StringSet& failedPaths);
        /**
         * @brief
         *
         * @param outfilename
         * @return bool
         */
        bool ConvertToVMAPModel(const char* outfilename);

        bool ok; /**< TODO */

        /**
         * @brief
         *
         * @param filename
         */
        Model(std::string& filename);
        /**
         * @brief
         *
         */
        ~Model() {_unload();}

    private:
        /**
         * @brief
         *
         */
        void _unload()
        {
            delete[] vertices;
            delete[] indices;
            vertices = NULL;
            indices = NULL;
        }
        std::string filename; /**< TODO */
        char outfilename;
};

/**
 * @brief
 *
 */
class ModelInstance
{
    public:
        Model* model; /**< TODO */

        uint32 id; /**< TODO */
        Vec3D pos, rot; /**< TODO */
        unsigned int d1, Scale;
        float w, sc;

        /**
         * @brief
         *
         */
        ModelInstance() {}
        /**
         * @brief
         *
         * @param f
         * @param ModelInstName
         * @param mapID
         * @param tileX
         * @param tileY
         * @param pDirfile
         */
        ModelInstance(MPQFile& f, const char* ModelInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile);

};

#endif
