/**
 * @file CitySceneGenerator.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "CitySceneGenerator.h"

#include "Scene.h"
#include "Material.h"
#include "Common.h"
#include "Helpers.h"
#include "Mesh.h"
#include "Node.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>

struct BuildingVertex
{
	float pos[3];
	float normal[3];
};


static const BuildingVertex buildingVertices[] = {
	{{1.0f, 1.0f, 0.0f}, {0.5773502f, 0.5773502f, 0.5773502f}},
	{{-1.0f, 1.0f, 0.0f}, {-0.5773502f, 0.5773502f, 0.5773502f}},
	{{-1.0f, 1.0f, 2.0f}, {-0.5773502f, 0.5773502f, 0.5773502f}},
	{{1.0f, 1.0f, 2.0f}, {0.5773502f, 0.5773502f, 0.5773502f}},
	{{1.0f, -1.0f, 0.0f}, {0.5773502f,- 0.5773502f, 0.5773502f}},
	{{-1.0f, -1.0f, 0.0f}, {-0.5773502f, -0.5773502f, 0.5773502f}},
	{{-1.0f, -1.0f, 2.0f}, {-0.5773502f, -0.5773502f, 0.5773502f}},
	{{1.0f, -1.0f, 2.0f}, {0.5773502f, -0.5773502f, 0.5773502f}}
};

static const uint32_t buildingIndices[] = {
	0, 1, 2,
	0, 2, 3,
	1, 5, 6,
	1, 6, 2,
	2, 6, 7,
	2, 7, 3,
	3, 7, 4,
	3, 4, 0,
	4, 7, 6,
	4, 6, 5
};

class Building : public AbstractNode
{
public:
	Building(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material) 
		: AbstractNode(mesh, material) { }

	void calculateBBox() {
		glm::vec3 min, max;
		for (int i = 0; i < 8; ++i) {
			const float* p = buildingVertices[i].pos;
			glm::vec4 ppos = modelMatrix() * glm::vec4(p[0], p[1], p[2], 1.0f);
			glm::vec3 point = glm::swizzle<glm::X, glm::Y, glm::Z>(ppos);
			if (i == 0) {
				min = point;
				max = point;
			} else {
				if (point.x > max.x)
					max.x = point.x;
				if (point.y > max.y)
					max.y = point.y;
				if (point.z > max.z)
					max.z = point.z;

				if (point.x < min.x)
					min.x = point.x;
				if (point.y < min.y)
					min.y = point.y;
				if (point.z < min.z)
					min.z = point.z;
			}
		}

		m_bbox = BoundingBox(min, max);
		m_centroid = min + ((max - min) / 2.0f);
	}

	virtual BoundingBox boundingBox() const {
		return m_bbox;
	}

	virtual glm::vec3 centroid() const {
		return m_centroid;
	}
private:
	BoundingBox m_bbox;
	glm::vec3 m_centroid;
};

CitySceneGenerator::CitySceneGenerator() {
	std::random_device rd;
	m_rng.seed(rd());
}

void CitySceneGenerator::generate(Scene* scene) {
	auto renderer = scene->renderer();
	auto material = std::make_shared<PhongMaterial>(renderer);
	material->setShader(renderer->shaderManager()->getGlslProgram("phong"));

	PhongMaterialData materialData = { glm::vec4(0.0f, 0.1f, 0.0f, 1.0f), 
		glm::vec4(0.8f, 0.3f, 0.1f, 1.0f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 5.0f };

	material->properties()->setData(materialData);
	material->properties()->flushData();

	auto mesh = std::make_shared<Mesh>();
	mesh->setPrimitiveType(PrimitiveType::TriangleList);

	size_t buildingVerticesCount = sizeof(buildingVertices) / sizeof(*buildingVertices);
	std::vector<char> vertices(reinterpret_cast<const char*>(buildingVertices), 
		reinterpret_cast<const char*>(buildingVertices) + sizeof(buildingVertices));
	std::vector<VertexElement> layout = create_vector<VertexElement>
		(VertexElement(3, VertexElementType::Float))(VertexElement(3, VertexElementType::Float));
	mesh->loadVertices(vertices, buildingVerticesCount, layout);

	size_t buildingIndicesCount = sizeof(buildingIndices) / sizeof(*buildingIndices);
	std::vector<uint32_t> indices(reinterpret_cast<const unsigned*>(buildingIndices), 
		reinterpret_cast<const unsigned*>(buildingIndices) + buildingIndicesCount);
	mesh->loadIndices(indices);

	size_t numBuildings = 100;
	float citySize = 200.0f;
	float minBuildingSize = 10.0f;
	float maxBuildingSize = 60.0f;
	float minHeightToWidthRatio = 8.0f;
	float maxHeightToWidthRatio = 16.0f;

	std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
	std::uniform_real_distribution<float> positionDist(-citySize, citySize);
	std::uniform_real_distribution<float> canonicalDist;

	std::vector<std::shared_ptr<AbstractNode>> buildings;
	for (size_t i = 0; i < numBuildings; i++) {
		auto building = std::make_shared<Building>(mesh, material);

		// set random position
		glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(positionDist(m_rng), positionDist(m_rng), 0.0f)
		);

		// rotate around z with random angle
		model = glm::rotate(model, angleDist(m_rng), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 scale;
		// multiplying uniform distribution will generate beta distribution
		scale.x = canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng)
			* (maxBuildingSize - minBuildingSize) + minBuildingSize;
		scale.y = scale.x;
		scale.z = canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng) * scale.x
			* (maxHeightToWidthRatio - minHeightToWidthRatio) + minHeightToWidthRatio;
		model = glm::scale(model, scale);

		building->setModelMatrix(model);
		building->calculateBBox();

		buildings.push_back(building);
	}

	scene->setStaticGeometry(std::move(buildings));
}
