/*
	katze 05/02/27
	コメントや空白をスキップする
*/
#pragma once

namespace BMW{
namespace Parser{
using namespace boost::spirit;

struct skip_comment : public grammar<skip_comment>
{/**
	XML形式のコメント(<!-- -->)や空白を認識するパーサー
	つまり、こいつをskipパーサーにしてやれば、空白だけなくコメントもスキップしながら
	パージングしてくれる
 */
	template<typename S>
	struct definition
	{
		definition(const skip_comment& self)
		{
			start_ = space_p | comment_p("<!--","-->");
		}

		rule<S> start_;
		const rule<S>& start(){ return start_; }
	};
};

} // namespace Parser end
} // namespace BMW end