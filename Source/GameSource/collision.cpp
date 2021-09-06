

#include "collision.h"



bool Collision::BallvsBall(
    const DirectX::XMFLOAT3& pivotA,
    const float radiusA_,
    const DirectX::XMFLOAT3& pivotB,
    const float radiusB_,
    DirectX::XMFLOAT3& outPosB
)
{
    DirectX::XMVECTOR PivotA      = DirectX::XMLoadFloat3(&pivotA);
    DirectX::XMVECTOR PivotB      = DirectX::XMLoadFloat3(&pivotB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PivotA, PivotB);
    DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    float lengthsq;
    DirectX::XMStoreFloat(&lengthsq, LengthSq);

    float range = radiusA_ + radiusB_;
    if (range * range < lengthsq) return false;

    // 当たらないrange分押し出す
    Vec = DirectX::XMVector3Normalize(Vec);
    DirectX::XMVECTOR outVec = DirectX::XMVectorScale(Vec, -1.0f * range); // 押し当ててる逆ベクトルにする
    DirectX::XMStoreFloat3(&outPosB, DirectX::XMVectorAdd(PivotA, outVec));

    // position.yは　pivotの半径分下にあるので
    outPosB.y += -radiusB_;

    return true;
}


bool Collision::RayPickvsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result
)
{
    // 以前の地面処理
    /*if (end.y < 0.0f)
    {
        result.position.x = end.x;
        result.position.y = 0.0f;
        result.position.z = end.z;
        result.normal.x = 0.0f;
        result.normal.y = 1.0f;
        result.normal.z = 0.0f;
        return true;
    }*/


    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes()) // モデルにメッシュがある限り????
    {
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);// メッシュのノード総数取得?

        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
        const DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);




        // レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        // --- 三角形 --- //
        // 頂点情報
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        // ポリゴン
        const std::vector <UINT>& indices = mesh.indices;

        int meterialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;

        // メッシュのサブセットで当たり判定を取る
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            // ポリゴンごとに判定
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // 三角形の三辺ベクトル算出

                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // 外積で法線を算出
                DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

                // 外積が0になっているとき
                DirectX::XMFLOAT3 normalized;
                DirectX::XMStoreFloat3(&normalized, Normal);
                if (normalized.x == 0.0f &&
                    normalized.y == 0.0f &&
                    normalized.z == 0.0f)
                {
                    DirectX::XMFLOAT3 bc;
                    DirectX::XMStoreFloat3(&bc, BC);
                    bc.x += 0.001f;
                    bc.y += 0.0f; // 増やす必要ない
                    bc.z += 0.0f; // 増やす必要ない
                    Normal = DirectX::XMVector3Cross(AB, DirectX::XMVectorSet(bc.x, bc.y, bc.z, 1.0f));
                }

                // 内積の結果がプラスなら裏向き //鋭角なら裏向き、鈍角なら表向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Normal, Dir);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot > 0.0f) continue;


                // 交点を算出
                // 射影1
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);


                //// 最初
                //DirectX::XMVECTOR T = DirectX::XMVector3Dot(V, Dir); 

                    // 先輩
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);

                float t;
                DirectX::XMStoreFloat(&t, T);
                //t = t / (dot + t);
                if (t < .0f || t > neart) continue;

                // 交点
                /*DirectX::XMVECTOR SUM = DirectX::XMVectorAdd( DirectX::XMVectorSet(dot, dot, dot, dot), DirectX::XMVectorSet(t, t, t, t) );
                DirectX::XMVECTOR X = DirectX::XMVectorDivide( DirectX::XMVectorSet(t, t, t, t), SUM );*/

                //DirectX::XMStoreFloat(&x, X);
                DirectX::XMVECTOR Position = DirectX::XMVectorAdd(Start, DirectX::XMVectorScale(Dir, t));


                // 交点が三角形の内側にあるか判定 
                // 1
                DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(Position, A);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(AB, V1);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f) continue;

                // 2
                DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(Position, B);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(BC, V2);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f) continue;

                // 3
                DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(Position, C);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(CA, V3);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f) continue;

                // 最近距離を更新
                neart = t;

                HitPosition = Position;
                HitNormal = Normal;
                meterialIndex = subset.materialIndex;


            }

        }

        // マテリアル番号があれば
        if (meterialIndex >= 0)
        {
            // ローカル空間からワールド空間へ変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            // ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = meterialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }


        }


    }


    return hit;
}